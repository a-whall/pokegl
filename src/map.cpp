#include "map.h"
using std::unordered_map;
#include "scene.h"
#include "debug.h"
using namespace Debug;
#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "collision.h"
#include "pair.h"


// TODO: width and height scaling of maps
// TODO: shift of maps based on neighbors and instance id
// TODO: active texture-object, texture-unit and ibuf management
// TODO: texture object array access via Tex_ID


Map_Manager::Map_Manager(Scene::Manager &man)
: scene_manager(man),
  map_shader(man.new_shader("shader/MapSprite.glsl"))
{
  obj_identify(map,alloc,this,"Map_Manager");

  ibuf=
  {
    1.0f, 1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 0.0f
  };

  glGenBuffers(1, &ivb);
  glBindBuffer(GL_ARRAY_BUFFER, ivb);
  glBufferData(GL_ARRAY_BUFFER, ibuf.size() * sizeof(float), ibuf.data(), GL_DYNAMIC_DRAW);

  GLuint sva = scene_manager.va;
  glVertexArrayVertexBuffer(sva, 2, ivb, 0, 16);
  glEnableVertexArrayAttrib(sva, 3);
  glVertexArrayAttribFormat(sva, 3, 4, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribBinding(sva, 3, 2);
  glVertexArrayBindingDivisor(sva, 2, 1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  World_Node * active_wnode = scene_manager.world_graph->get_current_node();

  if (active_wnode == nullptr)
    log_error_abort(map,"manager expected the world_graph to have a current world node");

  glGenTextures(max_tex_id, t);
  for (int i = 0; i < max_tex_id; ++i)
  {
    int w,h;
    Tex_ID tID = static_cast<Tex_ID>(i);
    log_from(map,"loading texture ",tex_str[tID]);
    // "complete" the texture object construction
    glBindTexture(GL_TEXTURE_2D, t[i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // upload the texture image to GPU
    GLubyte * image_data= load_textures( {get_tex_path(tID)}, map, this, &w, &h, 3 );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    delete image_data;
    // store texture bounds for later
    Texture_Data::def_bounds(tID, w/16, h/16);
  }

  update_maps(man.world_graph->get_current_node());
}



Map_Manager::~Map_Manager()
{
  glDeleteBuffers(1, &ivb);
  glDeleteTextures(max_tex_id, t);
}



void Map_Manager::update(float timestep, const Uint8 * key_states)
{
  if (key_states != nullptr && key_states[SDL_SCANCODE_M] && counter == 0) {
    counter = 16;
  }
  if (counter > 0)
    --counter;
}



void Map_Manager::render()
{
  glUseProgram(map_shader.handle);
  Camera *c = scene_manager.camera_controller;
  map_shader.set("cam_offset", glm::vec2(c->get_position().x, c->get_position().y));
  glBindVertexArray(scene_manager.va);
  glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 1); // Note: fixed a bug where calling glDrawElements or glDrawElementsInstanced crashed the program with a segfault. The reason was that I forgot to call glVertexAttribBinding. I think binding index may default to 0 without that call. So the map Shader was expecting to find (and read from) attrib location 3 on the buffer bound to vao index 0, which was only supplying data to attrib location 0 and 1. Thus it was trying to read memory out of bounds.
  glBindVertexArray(0);
  glUseProgram(0);
}



// TODO update this to have a local string stream, that combines variables instead of this hardcoded stuff: e.g.: <map_dir>/ << map_str[tID] << .png
const char* get_tex_path(Tex_ID tID)
{
  switch(tID)
  {
    case new_bark_town_tex:     return "assets/map/new_bark_town.png";
    case player_house_fl1_tex:  return "assets/map/player_house_fl1.png";
    case player_house_fl2_tex:  return "assets/map/player_house_fl2.png";
    case elms_lab_tex:          return "assets/map/elms_lab.png";
    case elms_house_tex:        return "assets/map/elms_house.png";
    case house_1_tex:           return "assets/map/house_1.png";
    case route_29_tex:          return "assets/map/route_29.png";
    case cherry_grove_city_tex: return "assets/map/cherry_grove_city.png";
    case route_30_tex:          return "assets/map/route_30.png";
    case pokecenter_tex:        return "assets/map/pokecenter.png";
    case pokemart_tex:          return "assets/map/pokemart.png";
    default: return "file due to get_asset_path error"; // makes sense for call to SDL_GetError()
  }
}

const char* get_tex_path(Map_ID mID)
{
  return get_tex_path(get_map_tex(mID));
}

// This function is for when the current world_node of the scene_managers world_graph is updated. 
// It is responsible for adjusting which texture objects are bound to which texture units, 
// and for updating the instanced vertex buffer from which the MapSprite shader reads scale and shift.
// @param curr_node: the new current world node with which to model the update.
void Map_Manager::update_maps(World_Node * curr_node)
{
  Map_ID m;
  int uo= 0; // multi-texture unit offset
  for (int i= 0; i < 5; ++i) // i tells u which direction
  {
    m= curr_node->mID[i];
    if (m != null_map_id) {
      glActiveTexture(GL_TEXTURE1 + uo);
      glBindTexture(GL_TEXTURE_2D, t[get_map_tex(m)]);
      Dim2D bound = Texture_Data::get_bounds(m);
      float sx= bound.w()/5.0f;
      float sy= bound.h()/5.0f;
      // TODO: fix t, 0 initialization
      float tx= 0.0f; //
      float ty= 0.0f;
      ibuf[uo*4+0]= sx;
      ibuf[uo*4+1]= sy;
      ibuf[uo*4+2]= tx;
      ibuf[uo*4+3]= ty;
      uo += 1;
    }
  }
  glNamedBufferSubData(ivb, 0, ibuf.size() * sizeof(float), ibuf.data());
}