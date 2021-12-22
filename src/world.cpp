#include "world.h"
#include <iostream>

static bool world_already_loaded = false;

World_node* load_world(Map_ID_enum mID)
{
  static World_node* nbt_nbt; //= new World_node(new_bark_town);
  static World_node* r29_nbt; //= new World_node(route_29_nbt);
  static World_node* r29_cgc; //= new World_node(route_29_cgc);
  static World_node* cgc_cgc; //= new World_node(cherry_grove_city);

  if (!world_already_loaded)
  {
    std::cout << "first call\n";
    // construct the entire set of world nodes
    nbt_nbt = new World_node(new_bark_town);
    r29_nbt = new World_node(route_29_nbt);
    r29_cgc = new World_node(route_29_cgc);
    cgc_cgc = new World_node(cherry_grove_city);

    // connect the entire set of world nodes
    nbt_nbt->left = r29_nbt;
    r29_nbt->left = r29_cgc;
    r29_cgc->left = cgc_cgc;

    cgc_cgc->right = r29_cgc;
    r29_cgc->right = r29_nbt;
    r29_nbt->right = nbt_nbt;
    
    // update the static flag that world data has been loaded
    world_already_loaded = true;
  }
  // return handle
  switch(mID)
  {
    case new_bark_town:     return nbt_nbt;
    case route_29_nbt:      return r29_nbt;
    case route_29_cgc:      return r29_cgc;
    case cherry_grove_city: return cgc_cgc;
  }
}