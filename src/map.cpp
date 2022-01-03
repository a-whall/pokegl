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
  Debug::log_from(Debug::map, "getting asset path (mID:",+mID,' ',to_str[mID],')');
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
: Sprite(0.0f, 0.0f, 0.0f, cam, shader)
{
  init_texture();
  if (mID != null_map_id)
  {
    load_texture(get_asset_path(mID));
  }
  init_buffers();
  glUseProgram(shader.handle);
  shader.set("Tex1",0);
  glUseProgram(0);
  model= glm::translate(model, glm::vec3((float)(w >> 5) - 0.25f, (float)(h >> 5) - 0.25f, 0.0f)); // shift map s.t. 0,0 be the middle of the bottom-left-most tile
  current_mID= mID;
}

Map::~Map()
{
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glDeleteVertexArrays(1, &vao);
}

void Map::load_texture(const char* mapTexPath)
{
  SDL_Surface *loaded_image = IMG_Load(mapTexPath);
  if(loaded_image == nullptr)
    Debug::log_error_abort("[Map] init_texture error: ",SDL_GetError());
  w_tiles = (w = loaded_image->w) >> 4;
  h_tiles = (h = loaded_image->h) >> 4;
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, current_gltexID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, loaded_image->pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  SDL_FreeSurface(loaded_image);
}

void Map::init_texture()
{
  glGenTextures(1, &current_gltexID);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, current_gltexID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Map::init_buffers()
{
  float hw = (float)(w >> 5); // half width (w_pix / 16 / 2)
  float hh = (float)(h >> 5); // half height (in tiles)
  unsigned ebo_data[] =
  {
    0, 1, 2,
    2, 3, 0
  };
  float vbo_data[]=
  {
   -hw,  hh, 0.0f,      0.0f, 0.0f,
    hw,  hh, 0.0f,      1.0f, 0.0f,
    hw, -hh, 0.0f,      1.0f, 1.0f,
   -hw, -hh, 0.0f,      0.0f, 1.0f
  };
  this->n_verts= 6; // num elements in ebo
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &ebo);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ebo_data), ebo_data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_data), vbo_data, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 20, (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, false, 20, (void*)12);
  glBindVertexArray(0);
}

void Map::update(float t, const Uint8* keystates)
{
  // Update maps model view matrix
  mv = cam.get_WorldToView_Matrix() * model;
}

void Map::render()
{
  if (is_visible) {
  glUseProgram(shader.handle);
  shader.set("MVP", cam.get_ViewToProjection_Matrix() * mv);
  glActiveTexture(GL_TEXTURE0);           
  glBindTexture(GL_TEXTURE_2D, current_gltexID);
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, n_verts, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  glUseProgram(0);
  }
}

void Map::translate(float x, float y)
{ // shifts an identity matrix by the given offsets, so previous shifts will be overwritten
  float hw = (float)(w >> 5); // half width ( x/16 /2)
  float hh = (float)(h >> 5); // half height
  model= glm::translate(glm::mat4(1.0f), glm::vec3(hw + x - 0.25f, hh + y - 0.0625f, 0.0f));
}

void Map::change(Map_ID_enum mID)
{
  SDL_Surface* li = IMG_Load(get_asset_path(mID));          // loaded image
  if (li == nullptr)
    Debug::log_error_abort("[Map] change error: ",SDL_GetError()); // error, abort
  w_tiles = (w = li->w) >> 4;                               // update class data
  h_tiles = (h = li->h) >> 4;
  glActiveTexture(GL_TEXTURE0);                                  
  glBindTexture(GL_TEXTURE_2D, current_gltexID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, li->pixels);
  SDL_FreeSurface(li);
  float hw = (float)(w >> 5); // half width ( x/16 /2)
  float hh = (float)(h >> 5); // half height
  glUseProgram(shader.handle);
  shader.set("Tex1", 0);
  glUseProgram(0);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  float vbo_data[]=
  {// XYZ Position       TexCoords
    -hw,  hh, 0.0f,      0.0f, 0.0f,
     hw,  hh, 0.0f,      1.0f, 0.0f,
     hw, -hh, 0.0f,      1.0f, 1.0f,
    -hw, -hh, 0.0f,      0.0f, 1.0f
  };
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vbo_data), vbo_data);
  glBindVertexArray(0);
  model= glm::translate(glm::mat4(1.0f), glm::vec3(hw - 0.25f, hh - 0.0625f, 0.0f));
  mv= cam.get_WorldToView_Matrix() * model;
  current_mID= mID;
}