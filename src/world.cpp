#include "world.h"
#include <iostream>
#include "debug.h"
#include "collision.h"
#include "sound.h"
#include "texture.h"



using namespace Debug;



World_Node::World_Node(Map_ID_enum map_ID)
{
  // object identification handled by World Graph
  mID[0] = map_ID;
};



World_Node::~World_Node()
{
  // object identification handled by World Graph
}



Map_ID World_Node::get_mID()
{
  return mID[0];
}



void World_Node::set_neighbors(Map_ID_enum up, int u_off, Map_ID_enum left, int l_off, Map_ID_enum down, int d_off, Map_ID_enum right, int r_off)
{
  // if this world node has neighbors then it is part of the overworld by design,
  // non-overworld maps will only need to be rendered one at a time, so no need to reference neighbors
  in_overworld = true;
  mID[1] = up;
  mID[2] = left;
  mID[3] = down;
  mID[4] = right;
  map_offsets[0] = u_off;
  map_offsets[1] = l_off;
  map_offsets[2] = d_off;
  map_offsets[3] = r_off;
}



World_Graph::World_Graph()
{
  obj_identify(world,alloc,this,"Graph");

  // construct full set of nodes, a node for all static Map_ID values.
  for (std::uint8_t i = 1U; i < route_31; ++i)
  {
    Map_ID_enum m = static_cast<Map_ID_enum>(i);
    world_map[m] = new World_Node(m);
    if (world & output_filter)
      log<std::cout>("\t      ",yellow,":",reset,obj_addr(world_map[m])," (node) ",map_str[ world_map[m]->get_mID() ]);
  }

  // build adjacency list for overworld maps
  world_map[new_bark_town]    ->set_neighbors(null_map_id,0,  route_29,0,          null_map_id,0,        null_map_id,0); // TODO: fix neighbors. set to nullmid for testing
  world_map[route_29]         ->set_neighbors(null_map_id,0,  cherry_grove_city,0, null_map_id,0,        new_bark_town,0); // TODO: fix neighbors. set to nullmid for testing
  world_map[cherry_grove_city]->set_neighbors(route_30,0,     null_map_id,0,       null_map_id,0,        route_29,0); // TODO: fix neighbors. set to nullmid for testing
  world_map[route_30]         ->set_neighbors(null_map_id,0,  null_map_id,0,       cherry_grove_city,0,  null_map_id,0); // TODO: same as above
//   world_map[route_31]->set_neighbors(null_map_id,0, violet_city,0, route_30,0, null_map_id,0);
//   world_map[violet_city]->set_neighbors(null_map_id,0, route_36,0, route_32,0, route_31,0);
//   world_map[route_32]->set_neighbors(violet_city,0, ruins_of_alph,0, null_map_id,0, null_map_id,0);
//   world_map[route_33]->set_neighbors(null_map_id,0, azalea_town,0, null_map_id,0, null_map_id,0);
//   world_map[azalea_town]->set_neighbors(null_map_id,0, null_map_id,0, null_map_id,0, route_33,0);
//   world_map[route_34]->set_neighbors(goldenrod_city,0, null_map_id,0, null_map_id,0, null_map_id,0);
//   world_map[route_35]->set_neighbors(null_map_id,0, null_map_id,0, goldenrod_city,0, route_36,0);
//   world_map[route_36]->set_neighbors(route_37,0, route_35,0, ruins_of_alph,0, violet_city,0);
//   world_map[route_37]->set_neighbors(ecruteak_city,0, null_map_id,0, route_36,0, null_map_id,0);
//   world_map[ecruteak_city]->set_neighbors(route_38,0, null_map_id,0, route_37,0, route_42,0);
//   world_map[route_38]->set_neighbors(null_map_id,0, farm,0, null_map_id,0, ecruteak_city,0);
//   world_map[farm]->set_neighbors(null_map_id,0, null_map_id,0, route_39,0, route_38,0);
//   world_map[route_39]->set_neighbors(farm,0,null_map_id,0,olivine_city,0,null_map_id,0);
//   world_map[olivine_city]->set_neighbors(route_39,0,route_40,0,null_map_id,0,null_map_id,0);
//   world_map[route_40]->set_neighbors(null_map_id,0,null_map_id,0,route_41,0,olivine_city,0);
//   world_map[route_41]->set_neighbors(route_40,0, cianwood_city,0, null_map_id,0, null_map_id,0);
//   world_map[route_42]->set_neighbors(null_map_id,0, ecruteak_city,0, null_map_id,0, mahogany_town,0);
//   world_map[mahogany_town]->set_neighbors(route_43,0, route_42,0, null_map_id,0, route_44,0);
//   world_map[route_43]->set_neighbors(lake_of_rage,0, null_map_id,0, mahogany_town,0, null_map_id,0);
//   world_map[lake_of_rage]->set_neighbors(null_map_id,0, null_map_id,0, route_43,0, null_map_id,0);
//   world_map[route_44]->set_neighbors(null_map_id,0, mahogany_town,0, null_map_id,0, null_map_id,0);
//   world_map[blackthorn_city]->set_neighbors(null_map_id,0, null_map_id,0, route_45,0, null_map_id,0);
//   world_map[route_45]->set_neighbors(blackthorn_city,0, null_map_id,0, route_46,0, null_map_id,0);
//   world_map[route_46]->set_neighbors(route_45,0, null_map_id,0, route_29,0, null_map_id,0);
}



World_Graph::~World_Graph()
{
  if (world & output_filter) log_from(object,dealloc);
  for (auto& node : world_map) {
    if (world & output_filter)
      log<cout>("\t\t",yellow,":",reset,obj_addr(node.second)," (node) ",map_str[ node.second->get_mID() ]);
    delete node.second;
  }
  obj_identify(world,dealloc,this,"Graph");
}



World_Node * World_Graph::set_current_node(Map_ID_enum mID)
{
  World_Node * next = get_node(mID);
  if (next == nullptr)
    log_error_abort(world,"failed to set current node. get_node(",map_str[mID],") returned nullptr");
  current_node = next;
  return current_node;
}



World_Node * World_Graph::get_current_node()
{
  return current_node;
}



World_Node * World_Graph::operator[](Map_ID mID)
{
  return get_node(mID);
}



World_Node* World_Graph::get_node(Map_ID_enum mID)
{
  return world_map.find(mID) != world_map.end() ? world_map.at(mID) : nullptr;
}



// TODO update this to have a local string stream, that combines variables instead of this hardcoded stuff: e.g.: <map_dir>/ << map_str[mID] << .png
const char* get_asset_path(Map_ID_enum mID)
{
  log_from(map,"getting asset path: ",map_str[mID]);
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
    default: return "file due to get_asset_path error"; // makes sense for call to SDL_GetError()
  }
}