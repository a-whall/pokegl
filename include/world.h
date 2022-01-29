#pragma once
#include "mapid.h"
#include <unordered_map>
#include <GL/glew.h>

struct World_Node
{
  Map_ID mID[5]= {null_map_id}; // 0 = self, 1 = up, 2 = left, 3 = down, 4 = right
  int map_offsets[4]= {0};
  bool in_overworld=false;

  World_Node(Map_ID_enum);
  ~World_Node();

  Map_ID get_mID();
  void set_neighbors(Map_ID up, int u_off, Map_ID left, int l_off, Map_ID down, int d_off, Map_ID right, int r_off);
};

class World_Graph
{
  World_Node * current_node;
  std::unordered_map< Map_ID_enum, World_Node * > world_map;
public:
  World_Graph();
  ~World_Graph();
  World_Node * operator[](Map_ID_enum);
  World_Node * set_current_node(Map_ID_enum);
  World_Node * get_current_node();
  World_Node * get_node(Map_ID_enum);
};