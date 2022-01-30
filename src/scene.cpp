#include <algorithm>
#include "scene.h"
#include "gtc/matrix_transform.hpp"
#include "world.h"

using namespace Debug;

namespace Scene
{



  void Manager::init_camera(int w, int h)
  {
    camera_controller = new Camera();
  }



  Player& Manager::new_player(Shader& s)
  {
    sprites.push_back(new Player(*this, s));
    return *sprites.back();
  }



  void Manager::init_text()
  {
    text_manager= new Text_Manager(*this);
  }



  void Manager::init_maps()
  {
    map_manager = new Map_Manager(*this);
  }



  // initializes the one and only vertex array as well as 
  void Manager::init_buffers()
  {

    unsigned eb_data[]=
    {
      0, 1, 2,
      2, 3, 0
    };

    float cvb_data[]=
    {
      -.5f, +.5f,       0.f, 0.f,
      +.5f, +.5f,       1.f, 0.f,
      +.5f, -.5f,       1.f, 1.f,
      -.5f, -.5f,       0.f, 1.f
    };

    float blvb_data[]=
    {
       0.f, +1.f, // top-left
      +1.f, +1.f, // top-right
      +1.f,  0.f, // bot-right
       0.f,  0.f  // bot-left
    };

    // create scene vertex array object
    glGenVertexArrays(1, &va);

    // create buffer objects
    glGenBuffers(1, &eb);
    glGenBuffers(1, &cvb);
    glGenBuffers(1, &blvb);

    // bind the scene vertex array
    glBindVertexArray(va);

    // create the quad element buffer, this automatically binds to the currently bound vertex array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(eb_data), eb_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // attach the element buffer to the vaobj
    glVertexArrayElementBuffer(va, eb);

    // create the center-aligned quad vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, cvb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cvb_data), cvb_data, GL_STATIC_DRAW);
    // DSA functions: affect currently bound Vertex Array Obj
    glBindVertexBuffer(0, cvb, 0, 16);
    glEnableVertexAttribArray(0);
    glVertexAttribFormat(0, 4, GL_FLOAT, false, 0);
    glVertexAttribBinding(0, 0);

    // create the bottom-left-aligned quad vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, blvb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(blvb_data), blvb_data, GL_STATIC_DRAW);
    // DSA functions: affect currently bound Vertex Array Obj
    glBindVertexBuffer(1, blvb, 0, 8); // stride = 8
    glEnableVertexAttribArray(2);
    glVertexAttribFormat(2, 2, GL_FLOAT, false, 0);
    glVertexAttribBinding(2, 1);

  }



  Shader& Manager::new_shader(const char* file_name)
  {
    shaders.push_back(new Shader(glCreateProgram()));
    compile(shaders.back()->handle, file_name);
    shaders.back()->log_program_resources();
    return *shaders.back();
  }



  void Manager::sync_world(Map_ID dest_mID)
  {
    Map_ID temp_mID = world_graph->get_current_node()->get_mID(); // store the current map id of the active world node

    World_Node * cnode = world_graph->set_current_node(dest_mID); // update the current node

    map_manager->update_maps(cnode);                              // update map textures and transform buffer

    if (get_area_track(temp_mID) != get_area_track(dest_mID))     // update music
      sound.play_song(get_area_track(dest_mID));
  }



  void Manager::update(float t)
  {
    for (auto& s : sprites) s->update(t, key_states);
    map_manager->update(t, key_states);
    text_manager->update(t, key_states);
  }



  void Manager::render()
  {
    // if the active world node is flagged as being 'in_overworld' then render all maps
    // otherwise print only the main map (which would be a self contained indoor location)
    // after that render sprites.

    map_manager->render();
    for (auto& s : sprites) s->render();

    // TODO: render grass, etc, things that need to be rendered over the player
    text_manager->render();
  }



  void Manager::clean()
  {
    delete world_graph;
    for (auto& s: sprites) delete s;
    delete map_manager;
    for (auto& s: shaders) delete s;
    delete text_manager;
  }



  void Manager::refresh()
  {
    using std::remove_if;
    using std::begin;
    using std::end;
    auto is_not_active = [](const Sprite* sprite) { return !sprite->is_active; };
    sprites.erase( remove_if(begin(sprites),end(sprites),is_not_active), end(sprites) );
  }
}