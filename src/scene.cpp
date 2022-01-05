#include <algorithm>
#include "scene.h"
#include "gtc/matrix_transform.hpp"
#include "world.h"

namespace Scene
{
  void Manager::init_camera(int x, int y)
  {
    float aspect_ratio = x / (float)y;
    this->camera_controller = new Camera(glm::vec3(0, 0, 5), glm::vec3(0, 0, -1), aspect_ratio);
  }

  Player& Manager::new_player(Shader& s)
  {
    Camera& c = *this->camera_controller;
    sprites.push_back(new Player(*this, s));
    sprites.back()->maps = this->maps;
    return *sprites.back();
  }

  Map& Manager::init_map(Shader& s)
  {
    World_Node * active_wnode = world_graph->get_current_node();
    if (active_wnode == nullptr)
      Debug::log_error_abort(Debug::scene,"manager expected the world_graph to have a current world node");
    Camera& c = *this->camera_controller;
    maps[0] = new Map(active_wnode->mID, c, s);
    maps[1] = new Map(null_map_id, c, s); // up
    maps[2] = new Map(null_map_id, c, s); // left
    maps[3] = new Map(null_map_id, c, s); // down
    maps[4] = new Map(null_map_id, c, s); // right
    
    return *maps[0];
  }

  void Manager::update_map(Map_ID_enum mID)
  {
    // This is a helper function that exchanges the map pointers to 3 of the 5 total map objects. This way, for example, when walking left onto a new map zone the maps which are already loaded on the gpu can be reused.
    // Note: I was writing this and trying to get it to work and these random translations by (0,0) worked mysteriously. I was just testing, I'm not sure why the same translation for all maps ...just works...
    auto cascade_maps = [&](size_t i1, size_t i2) {
      Map * tmp = maps[i1];
      maps[i1] = maps[0];
      maps[i1]->translate(0,0);
      maps[i1]->is_visible=true;
      maps[0] = maps[i2];
      maps[0]->translate(0,0);
      maps[0]->is_visible=true;
      maps[i2] = tmp;
      maps[i2]->change(mID);
      maps[i2]->translate(0,0);
      maps[i2]->is_visible=true;
    };
    World_Node * wnode = world_graph->get_current_node();
    if      (wnode->up() == mID) cascade_maps(3,1);
    else if (wnode->left() == mID) cascade_maps(4,2);
    else if (wnode->down() == mID) cascade_maps(1,3);
    else if (wnode->right() == mID) cascade_maps(2,4);
    maps[0]->change(mID); // this will naturally happen for non-overworld maps.
    World_Node * cnode = world_graph->set_current_node(mID);
    Map_ID_enum m; // temp value for the ifs below
    if (cnode->in_overworld) {
      Debug::log_from(Debug::scene,"cnode in overworld");
      if ((m=cnode->up()) != null_map_id) {    // if the new up map has null id according to world graph:
        if (maps[1]->current_mID != m)            // if the up map hasn't already been swapped by cascade_maps:
          maps[1]->change(m);                        // load the map
        maps[1]->translate(0,maps[0]->h_tiles);   // translate the map sprite appropriately
        maps[1]->update(0,nullptr);               // update the map object so that the correct matrix is used for rendering
        maps[1]->is_visible= true;                // set the map to be visible
      }
      else maps[1]->is_visible= false;         // else turn the up maps visibility off
      if ((m=cnode->left()) != null_map_id) {  // repeat
        if (maps[2]->current_mID != m)
          maps[2]->change(m);
        maps[2]->translate(-maps[2]->w_tiles,0);
        maps[2]->update(0, nullptr);
        maps[2]->is_visible= true;
      }
      else maps[2]->is_visible= false;
      if ((m=cnode->down()) != null_map_id) {
        if (maps[3]->current_mID != m)
          maps[3]->change(m);
        maps[3]->translate(0,-maps[3]->h_tiles);
        maps[3]->update(0,nullptr);
        maps[3]->is_visible= true;
      }
      else maps[3]->is_visible= false;
      if ((m=cnode->right()) != null_map_id) {
        if (maps[4]->current_mID != m)
          maps[4]->change(m);
        maps[4]->translate(maps[0]->w_tiles,0);
        maps[4]->update(0,nullptr);
        maps[4]->is_visible= true;
      }
      else maps[4]->is_visible= false;
    }
    else {// set neighbors invisible
      Debug::log_from(Debug::scene,"cnode is not in overworld");
      maps[0]->change(mID);
      maps[1]->is_visible= maps[2]->is_visible= maps[3]->is_visible= maps[4]->is_visible= false;
    }
  }

  Shader& Manager::new_shader(const char* file_name)
  {
    shaders.push_back(new Shader(glCreateProgram()));
    compile(shaders.back()->handle, file_name);
    log(shaders.back()->handle, GL_PROGRAM_INPUT);
    log(shaders.back()->handle, GL_UNIFORM);
    return *shaders.back();
  }

  void Manager::update(float t)
  {
    for (auto& m : maps)    m->update(t, key_states);
    for (auto& s : sprites) s->update(t, key_states);
  }

  void Manager::render()
  {
    // if the active world node is flagged as being 'in_overworld' then render all maps
    // otherwise print only the main map (which would be a self contained indoor location)
    // after that render sprites.
    if (world_graph->get_current_node()->in_overworld) {
      for (auto& m : maps) if (m->current_mID != null_map_id) m->render();
    }
    else {
      maps[0]->render();
    }
    for (auto& s : sprites) s->render();
    // TODO: render grass, etc, things that need to be rendered over the player
  }

  void Manager::clean()
  {
    for (auto& s: sprites) delete s;
    for (auto& m: maps)    delete m;
    for (auto& s: shaders) delete s;
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