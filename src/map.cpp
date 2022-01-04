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

const char* get_asset_path(Map_ID_enum mID)
{
  Debug::log_from(Debug::map,"getting asset path: ",to_str[mID]);
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
    default:                return "file due to get_asset_path error"; // makes sense for call to SDL_GetError()
  }
}

Map::Map(Map_ID_enum mID, Camera &cam, Shader &shader)
: Sprite(0.0f, 0.0f, 0.0f, cam, shader),
  current_mID (mID)
{
  Debug::log_from(Debug::map,"initializing: ",std::hex,this,std::dec);
  glGenVertexArrays(1, &this->vao);
  glGenBuffers(1, &this->ebo);
  glGenBuffers(1, &this->vbo);
  init_texture();
  try_to_load_texture();
  init_buffers();
  glUseProgram(shader.handle);
  shader.set("Tex1",0);
  glUseProgram(0);
  this->translate(0, -.1875); // shift map s.t. 0,0 be the middle of the bottom-left-most tile
}

Map::~Map()
{
  glDeleteBuffers(1, &this->vbo);
  glDeleteBuffers(1, &this->ebo);
  glDeleteVertexArrays(1, &this->vao);
  Debug::log_from(Debug::map,"deleting: ",std::hex,this,std::dec);
}

void Map::init_texture()
{
  glGenTextures(1, &this->current_gltexID);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->current_gltexID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Map::try_to_load_texture()
{
  if (this->current_mID == null_map_id) {
    Debug::log_from(Debug::map,"has null map id, texture-load cancelled");
    return;
  }
  SDL_Surface * loaded_image= IMG_Load(get_asset_path(current_mID));
  if(loaded_image == nullptr) {
    Debug::log_error_abort(Debug::map,"texture-load failed ",SDL_GetError());
  }
  this->w_tiles= (this->w= loaded_image->w) / 16;
  this->h_tiles= (this->h= loaded_image->h) / 16;
  Debug::log_from(Debug::map,"w,h: ", w_tiles,',',h_tiles);
  Collision_Data::try_to_def_bounds(current_mID, w_tiles, h_tiles);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->current_gltexID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, this->w, this->h, 0, GL_RGB, GL_UNSIGNED_BYTE, loaded_image->pixels);
  SDL_FreeSurface(loaded_image);
}

void Map::init_buffers()
{
  float w2 (w_tiles / 2);
  float h2 (h_tiles / 2);
  unsigned ebo_data[]=
  {
    0, 1, 2,
    2, 3, 0
  };
  float vbo_data[]= // (pos:vec3) (tex_coord:vec2)
  {
   -w2,  h2, 0.0f,       0.0f, 0.0f,
    w2,  h2, 0.0f,       1.0f, 0.0f,
    w2, -h2, 0.0f,       1.0f, 1.0f,
   -w2, -h2, 0.0f,       0.0f, 1.0f
  };
  this->n_verts= 6; // num elements in ebo
  Debug::log_from(Debug::map,"specifying buffers");
  glBindVertexArray(this->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ebo_data), ebo_data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_data), vbo_data, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 20, (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, false, 20, (void*)12);
  glBindVertexArray(0);
}

void Map::update(float t, const Uint8* keystates)
{
  mv= cam.get_WorldToView_Matrix() * model;
  if (keystates != nullptr && keystates[SDL_SCANCODE_M] && counter == 0) {
    counter = 16;
    Debug::log_from(Debug::map,std::hex,this,std::dec,": (",model[3].x,",",model[3].y,")");
  }
  if (counter > 0) --counter;
}

void Map::render()
{
  if (this->is_visible) {
    glUseProgram(shader.handle);
    shader.set("MVP", cam.get_ViewToProjection_Matrix() * this->mv);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->current_gltexID);
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->n_verts, GL_UNSIGNED_INT, 0);
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
  model= glm::translate(glm::mat4(1.0f), glm::vec3(hw + x - 0.25f, hh + y - 0.0625f, 0.0f));
}

// Transform this map to load data for a new zone.
// @param mID: The map ID of the data to load.
void Map::change(Map_ID_enum mID)
{
  Debug::log_from(Debug::map,"changing from ",to_str[current_mID]," to ",to_str[mID]);
  current_mID= mID;
  try_to_load_texture();
  float hw (this->w_tiles / 2);
  float hh (this->h_tiles / 2);
  float vbo_data[]=
  {// XYZ Position       TexCoords
   -hw,  hh, 0.0f,       0.0f, 0.0f,
    hw,  hh, 0.0f,       1.0f, 0.0f,
    hw, -hh, 0.0f,       1.0f, 1.0f,
   -hw, -hh, 0.0f,       0.0f, 1.0f
  };
  glBindVertexArray(this->vao);
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vbo_data), vbo_data);
  glBindVertexArray(0);
  this->model= glm::translate(glm::mat4(1.0f), glm::vec3(hw - 0.25f, hh - 0.0625f, 0.0f));
  this->mv= cam.get_WorldToView_Matrix() * this->model;
}