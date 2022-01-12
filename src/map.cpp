#include "map.h"
#include "shader.h"
#include "scene.h"
#include "camera.h"
#include "debug.h"
#include "SDL_image.h"
#include "gtc/matrix_transform.hpp"
#include "collision.h"
#include "warps.h"
#include "world.h"
#include "texture.h"



using namespace Debug;
using glm::mat4;
using glm::mat3;



const char* get_asset_path(Map_ID_enum);



Map::Map(Map_ID_enum mID, Camera &cam, Shader &shader)
: Sprite(0.0f, 0.0f, 0.0f, cam, shader),
  current_mID (mID)
{
  obj_identify(map,alloc,this,"Map");
  glGenVertexArrays(1, &va);
  glGenBuffers(1, &eb);
  glGenBuffers(1, &vb);
  generate_texture(GL_TEXTURE_2D, &t);
  try_to_load_texture();
  init_buffers();
  glUseProgram(shader.handle);
  shader.set("map_tex",0);
  glUseProgram(0);
  this->Map::translate(0, -.1875); // such that 0,0 becomes the middle of the bottom-left-most tile.
}



Map::~Map()
{
  glDeleteBuffers(1, &vb);
  glDeleteBuffers(1, &eb);
  glDeleteVertexArrays(1, &va);
  obj_identify(map,dealloc,this,"Map");
}



void Map::try_to_load_texture()
{
  if (current_mID == null_map_id) {
    log_from(map,"has null map id, texture-load cancelled");
    return;
  }
  GLubyte * image_data= load_textures( {get_asset_path(current_mID)}, map, this, &w, &h, 3 );
  w_tiles= w / 16;
  h_tiles= h / 16;
  Collision_Data::try_to_def_bounds(current_mID, w_tiles, h_tiles); // used for boundary detection
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, t);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
  glBindTexture(GL_TEXTURE_2D, 0);
  delete image_data;
}


void Map::init_buffers()
{
  // half width and half height in tiles (1 tile = 16 x 16 pixels)
  float w2 (w_tiles / 2);
  float h2 (h_tiles / 2);
  unsigned eb_data[]=
  {
    0, 1, 2,
    2, 3, 0
  };
  float vb_data[]= // (pos:vec3) (tex_coord:vec2)
  {
   -w2,  h2, 0.0f,       0.0f, 0.0f,
    w2,  h2, 0.0f,       1.0f, 0.0f,
    w2, -h2, 0.0f,       1.0f, 1.0f,
   -w2, -h2, 0.0f,       0.0f, 1.0f
  };
  n_verts= 6; // num elements in eb
  glBindVertexArray(va);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(eb_data), eb_data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vb_data), vb_data, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 20, (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, false, 20, (void*)12);
  glBindVertexArray(0);
}



void Map::update(float t, const Uint8* keystates)
{
  using namespace Debug;
  mv= cam.view() * model;
  if (keystates != nullptr && keystates[SDL_SCANCODE_M] && counter == 0) {
    counter = 16;
    log_from(map,std::hex,this,std::dec,": (",model[3].x,",",model[3].y,")");
  }
  if (counter > 0)
    --counter;
}



void Map::render()
{
  if (this->is_visible) {
    glUseProgram(shader.handle);
    shader.set("mapsprite_mvp", cam.projection() * mv);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, t);
    glBindVertexArray(va);
    glDrawElements(GL_TRIANGLES, n_verts, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0);
  }
}



// Translates the model of this map object in the x-y plane. This function overwrites the model matrix with the result of an identity matrix that has been translated by the given offsets, therefore any previous translations will be overwritten.
// @param x,y: shift the model matrix by this many units in the x and y directions respectively.
void Map::translate(float x, float y)
{
  float hw (this->w_tiles / 2);
  float hh (this->h_tiles / 2);
  model= glm::translate(mat4(1.0f), vec3(hw + x - 0.25f, hh + y - 0.0625f, 0.0f));
}



// Transform this map to load data for a new zone.
// @param mID: The map ID of the data to load.
void Map::change(Map_ID_enum mID)
{
  log_from(map,obj_addr(this),": changing from ",to_str[current_mID]," to ",to_str[mID]);

  current_mID= mID;

  try_to_load_texture();

  float hw (this->w_tiles / 2);
  float hh (this->h_tiles / 2);

  float vb_data[]=
  {// XYZ Position       TexCoords
   -hw, +hh, 0.0f,       0.0f, 0.0f,
   +hw, +hh, 0.0f,       1.0f, 0.0f,
   +hw, -hh, 0.0f,       1.0f, 1.0f,
   -hw, -hh, 0.0f,       0.0f, 1.0f
  };

  glBindVertexArray(va);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vb_data), vb_data);
  glBindVertexArray(0);

  model= glm::translate(mat4(1.0f), vec3(hw - 0.25f, hh - 0.0625f, 0.0f));
  mv= cam.view() * model;
}



const char* get_asset_path(Map_ID_enum mID)
{
  log_from(map,"getting asset path: ",to_str[mID]);
  switch(mID)
  {
    case new_bark_town:     return "assets/map/new_bark_town.png";
    case player_house_fl1:  return "assets/map/player_house_fl1.png";
    case player_house_fl2:  return "assets/map/player_house_fl2.png";
    case elms_lab:          return "assets/map/elms_lab.png";
    case elms_house:        return "assets/map/elms_house.png";
    case new_bark_house_1:  return "assets/map/nbt_house1.png";
    case route_29:          return "assets/map/route_29.png";
    case cherry_grove_city: return "assets/map/cherry_grove_city.png";
    case route_30:          return "assets/map/route_30.png";
    default: return "file due to get_asset_path error"; // makes sense for call to SDL_GetError()
  }
}