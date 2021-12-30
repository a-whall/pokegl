#pragma once
#include "mapid.h"
#include "frameid.h"

namespace Warp
{ 
  struct Destination
  {
    bool step_before;       // should player take a step before the warp
    bool step_after;        // should player take a step after the warp
    Map_ID_enum mID;        // the map id of the world node to warp to
    Frame_ID fID;           // an opportunity to specify which direction the player should be facing
    SpawnPoint spawn_point; // the position to place oneself on the new map as if it were a grid
  };

  struct Warp_Point
  {
    Point p;         // the location of the warp point in map-grid coordinates
    Destination dst; // flags and info about the warp and its destination
  };

  const Warp_Point * get_warp_table(Map_ID_enum);

  // Note: while num_warps[] remains to be fully implemented it can easily cause seg fault crash
  constexpr int num_warps[]=
  { // Map_ID_enum is used as an index to this array, so it must be in the exact same order
    0, // null_map_id
    4, // new_bark_town
    3, // player_house_fl1
    1, // player_house_fl2
    2, // elms_lab
    2, // elms_house
    2, // new_bark_house_1
    0, // route_29
    5, // cherry_grove_city ??
  };

  constexpr Warp_Point new_bark_town_warp_data[4]=
  {
    { {15,16}, {true, false, player_house_fl1, idle_u, {6,0}} },
    { {8, 18}, {true, false, elms_lab,         idle_u, {4,0}} },
    { {5, 10}, {true, false, new_bark_house_1, idle_u, {2,0}} },
    { {13, 8}, {true, false, elms_house,       idle_u, {2,0}} }
  };

  constexpr Warp_Point player_house_fl1_warp_data[3]=
  {
    { {6,-1}, {false, true, new_bark_town,    idle_d, {15,16}} }, // outside
    { {7,-1}, {false, true, new_bark_town,    idle_d, {15,16}} }, // outside
    { {9, 7}, {true,  true, player_house_fl2, idle_d, {7,5}} }    // upstairs
  };

  constexpr Warp_Point player_house_fl2_warp_data[1]=
  {
    { {7, 5}, {true, true, player_house_fl1, idle_d, {9, 7}} } // downstairs
  };

  constexpr Warp_Point elms_lab_warp_data[2]=
  {
    { {4, -1}, {false, true, new_bark_town, idle_d, {8,18}} },
    { {5, -1}, {false, true, new_bark_town, idle_d, {8,18}} }
  };

  constexpr Warp_Point new_bark_house_1_warp_data[2]=
  {
    { {2, -1}, {false, true, new_bark_town, idle_d, {5,10}} },
    { {3, -1}, {false, true, new_bark_town, idle_d, {5,10}} }
  };

  constexpr Warp_Point elms_house_warp_data[2]=
  {
    { {2, -1}, {false, true, new_bark_town, idle_d, {13,8}} },
    { {3, -1}, {false, true, new_bark_town, idle_d, {13,8}} }
  };

  // constexpr Warp_Point route_29[0]= {};

  constexpr Warp_Point cherry_grove_city_warp_data[5]=
  {
    { {17,14}, {true, false, cherry_grove_house_1, idle_u, {4,0}} },
    { {23,18}, {true, false, cherry_grove_mart,    idle_u, {4,0}} },
    { {25,12}, {true, false, cherry_grove_house_2, idle_u, {4,0}} },
    { {29,18}, {true, false, cherry_grove_pc,      idle_u, {4,0}} },
    { {31,10}, {true, false, cherry_grove_house_3, idle_u, {4,0}} }
  };
}