#pragma once
#include "sprite.h"
#include "mapid.h"

class World_Node;

const char* get_asset_path(Map_ID_enum mID);

class Map
: public Sprite
{
  GLuint vao, vbo, ebo, current_gltexID;

public:

  int w=32, h=32, w_tiles=0, h_tiles=0;
  Map_ID_enum current_mID;
  World_Node * world_node;
  bool is_visible = true;

  Map(Map_ID_enum mID, Camera &c, Shader &s);
  ~Map();

  void load_texture(const char* mapTexPath);
  void init_texture();
  void init_buffers() override;
  void update(float t, const Uint8* keystates) override;
  void render() override;
  void translate(float, float);
  bool allows_move_to(int x, int y);
  void change(Map_ID_enum mID);
};