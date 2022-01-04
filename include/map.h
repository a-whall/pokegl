#pragma once
#include "sprite.h"
#include "mapid.h"

class World_Node;

class Map
: public Sprite
{
  GLuint vao, vbo, ebo, current_gltexID;
public:
  int w=32, h=32, w_tiles=0, h_tiles=0;
  int counter=0;
  Map_ID_enum current_mID;
  World_Node * world_node;
  bool is_visible=true;

  Map(Map_ID_enum mID, Camera &c, Shader &s);
  ~Map();

  void update(float t, const Uint8* keystates) override;
  void render() override;
  void translate(float, float);
  void change(Map_ID_enum mID);
private:
  void init_buffers() override;
  void init_texture();
  void try_to_load_texture();
};