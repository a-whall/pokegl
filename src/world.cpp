#include "world.h"
#include <iostream>
#include "debug.h"
#include "collision.h"
#include "sound.h"

using namespace Debug;

World_Node::World_Node(Map_ID_enum map_ID)
: mID(map_ID)
{
  // object identification handled by World Graph
};

World_Node::~World_Node()
{
  // object identification handled by World Graph
}

void World_Node::set_neighbors(Map_ID_enum up, Map_ID_enum left, Map_ID_enum down, Map_ID_enum right)
{
  // if this world node has neighbors then it is part of the overworld by design,
  // non-overworld maps will only need to be rendered one at a time, so no need to reference neighbors
  in_overworld = true;
  neighbors[0] = up;
  neighbors[1] = left;
  neighbors[2] = down;
  neighbors[3] = right;
}

Map_ID_enum World_Node::up()    { return neighbors[0]; }
Map_ID_enum World_Node::left()  { return neighbors[1]; }
Map_ID_enum World_Node::down()  { return neighbors[2]; }
Map_ID_enum World_Node::right() { return neighbors[3]; }



World_Graph::World_Graph()
{
  obj_identify(world,alloc,this,"Graph");

  // construct full set of nodes, a node for all static Map_ID values.
  for (std::uint8_t i = 1U; i < max_map_id; ++i)
  {
    Map_ID_enum m = static_cast<Map_ID_enum>(i);
    world_map[m] = new World_Node(m);
    if (world & output_filter)
      log<std::cout>("\t      ",yellow,":",reset,obj_addr(world_map[m])," (node) ",to_str[world_map[m]->mID]);
  }
  // build adjacency list for overworld maps
  world_map[new_bark_town]->set_neighbors(null_map_id, route_29, null_map_id, null_map_id); // TODO: fix neighbors. set to nullmid for testing
  world_map[route_29]->set_neighbors(null_map_id, cherry_grove_city, null_map_id, new_bark_town); // TODO: fix neighbors. set to nullmid for testing
  world_map[cherry_grove_city]->set_neighbors(route_30, null_map_id, null_map_id, route_29); // TODO: fix neighbors. set to nullmid for testing
  world_map[route_30]->set_neighbors(null_map_id/*route_31*/, null_map_id, cherry_grove_city, null_map_id); // TODO: same as above
  world_map[route_31]->set_neighbors(null_map_id, violet_city, route_30, null_map_id);
  world_map[violet_city]->set_neighbors(null_map_id, route_36, route_32, route_31);
  world_map[route_32]->set_neighbors(violet_city, ruins_of_alph, null_map_id, null_map_id);
  world_map[route_33]->set_neighbors(null_map_id, azalea_town, null_map_id, null_map_id);
  world_map[azalea_town]->set_neighbors(null_map_id, null_map_id, null_map_id, route_33);
  world_map[route_34]->set_neighbors(goldenrod_city, null_map_id, null_map_id, null_map_id);
  world_map[route_35]->set_neighbors(null_map_id, null_map_id, goldenrod_city, route_36);
  world_map[route_36]->set_neighbors(route_37, route_35, ruins_of_alph, violet_city);
  world_map[route_37]->set_neighbors(ecruteak_city, null_map_id, route_36, null_map_id);
  world_map[ecruteak_city]->set_neighbors(route_38, null_map_id, route_37, route_42);
  world_map[route_38]->set_neighbors(null_map_id, farm, null_map_id, ecruteak_city);
  world_map[farm]->set_neighbors(null_map_id, null_map_id, route_39, route_38);
  world_map[route_39]->set_neighbors(farm,null_map_id,olivine_city,null_map_id);
  world_map[olivine_city]->set_neighbors(route_39,route_40,null_map_id,null_map_id);
  world_map[route_40]->set_neighbors(null_map_id,null_map_id,route_41,olivine_city);
  world_map[route_41]->set_neighbors(route_40, cianwood_city, null_map_id, null_map_id);
  world_map[route_42]->set_neighbors(null_map_id, ecruteak_city, null_map_id, mahogany_town);
  world_map[mahogany_town]->set_neighbors(route_43, route_42, null_map_id, route_44);
  world_map[route_43]->set_neighbors(lake_of_rage, null_map_id, mahogany_town, null_map_id);
  world_map[lake_of_rage]->set_neighbors(null_map_id, null_map_id, route_43, null_map_id);
  world_map[route_44]->set_neighbors(null_map_id, mahogany_town, null_map_id, null_map_id);
  world_map[blackthorn_city]->set_neighbors(null_map_id, null_map_id, route_45, null_map_id);
  world_map[route_45]->set_neighbors(blackthorn_city, null_map_id, route_46, null_map_id);
  world_map[route_46]->set_neighbors(route_45, null_map_id, route_29, null_map_id);
}

World_Graph::~World_Graph()
{
  if (world & output_filter) log_from(object,dealloc);
  for (auto& node : world_map) {
    if (world & output_filter)
      log<cout>("\t\t",yellow,":",reset,obj_addr(node.second)," (node) ",to_str[node.second->mID]);
    delete node.second;
  }
  obj_identify(world,dealloc,this,"Graph");
}

World_Node * World_Graph::set_current_node(Map_ID_enum mID)
{
  World_Node * next = get_node(mID);
  if (next == nullptr)
    log_error_abort(world,"failed to set current node. get_node(",to_str[mID],") returned nullptr");
  current_node = next;
  return current_node;
}

World_Node* World_Graph::get_current_node()
{
  return current_node;
}

World_Node* World_Graph::get_node(Map_ID_enum mID)
{
  return world_map.find(mID) != world_map.end() ? world_map[mID] : nullptr;
}