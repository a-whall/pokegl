#pragma once
#include "mapid.h"
#include <unordered_map>
#include <GL/glew.h>

struct World_Node
{
  const Map_ID_enum mID;
  Map_ID_enum neighbors[4];
  bool in_overworld=false;
  World_Node(Map_ID_enum);
  ~World_Node();
  void set_neighbors(Map_ID up, Map_ID left, Map_ID down, Map_ID right);
  Map_ID_enum up();
  Map_ID_enum left();
  Map_ID_enum down();
  Map_ID_enum right();
};

class World_Graph
{
  World_Node * current_node;
  std::unordered_map< Map_ID_enum, World_Node * > world_map;
public:
  World_Graph();
  ~World_Graph();
  World_Node * set_current_node(Map_ID_enum);
  World_Node * get_current_node();
  World_Node * get_node(Map_ID_enum);
};