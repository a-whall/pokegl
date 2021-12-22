#pragma once
#include "sprite.h"
#include "mapid.h"

class World_node;

const char* get_asset_path(Map_ID_enum mID);

class Map
: public Sprite
{
  GLuint vao, vbo, ebo, current_gltexID;
  const GLubyte* collision_ptr;

public:

  int w, h, w_tiles, h_tiles;
  Map_ID_enum current_mID;
  World_node *world_node;
  bool is_visible = true;

  Map(Map_ID_enum mID, Camera &c, Shader &s);
  ~Map();

  void init_texture(const char* mapTexPath);
  void init_buffers() override;
  void update(float t, const Uint8* keystates) override;
  void render() override;
  void shift_vertex_coords_by_offset(float, float);
  bool allows_move_to(int x, int y);
  void change(Map_ID_enum mID);
};