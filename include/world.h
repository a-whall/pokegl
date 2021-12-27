#pragma once
#include "mapid.h"
#include <unordered_map>

struct World_node
{
  Map_ID_enum mID;
  Map_ID_enum neighbors[4];
  bool in_overworld = false;

  World_node(Map_ID_enum);

  void set_neighbors(Map_ID_enum, Map_ID_enum, Map_ID_enum, Map_ID_enum);

  Map_ID_enum up();
  Map_ID_enum left();
  Map_ID_enum down();
  Map_ID_enum right();

};

World_node* load_world(Map_ID_enum);

class World_Graph
{
  std::unordered_map< Map_ID_enum, World_node* > world_map;

public:

  World_Graph();
  ~World_Graph();

  World_node* get_node(Map_ID_enum);
};