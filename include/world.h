#pragma once
#include "mapid.h"

struct World_node
{
  Map_ID_enum mID;
  World_node * left  = nullptr;
  World_node * right = nullptr;
  World_node * up    = nullptr;
  World_node * down  = nullptr;

  World_node(Map_ID_enum map_ID)
  {
    mID= map_ID;
  }
};

World_node* load_world(Map_ID_enum);