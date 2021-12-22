#pragma once
#include "mapid.h"
#include "frameid.h"
#include <unordered_map>

namespace Warp
{
  using std::unordered_map;
  //using Frame_ID;

  struct Dest
  {
    bool in_overworld;
    Map_ID_enum mID;
    Frame_ID fID;
    SpawnPoint spawn_point;
  };

  typedef struct
  {
    Point p;
    Dest dst;
  } Warp_point;

  constexpr int num_warps[]=
  {
    3, // player_house_fl1
    1, // player_house_fl2
    4, // new_bark_town
    0, // route_29_nbt
    0, // route_29_cgc
    0  // cherry_grove_city
  };

  constexpr Warp_point player_house_fl1_warp_data[3]=
  {
    { {6,-1}, {true,  new_bark_town,    e_down_idle, {23,12}} }, // outside
    { {7,-1}, {true,  new_bark_town,    e_down_idle, {23,12}} }, // outside
    { {9, 7}, {false, player_house_fl2, e_down_idle, {7,5}} }    // upstairs
  };

  constexpr Warp_point player_house_fl2_warp_data[1]=
  {
    { {7, 5}, {false, player_house_fl1, e_down_idle, {9, 7}} } // downstairs
  };

  constexpr Warp_point new_bark_town_warp_data[4]=
  {
    { {23,12}, {false, player_house_fl1, e_up_idle, {6,-1}} }, // player house fl1
    { {16,14}, {false, elms_lab,         e_up_idle, {4,-1}} }, // elms lab
    { {13, 6}, {false, nbt_house_1,      e_up_idle, {4,-1}} }, // nbt house 1
    { {21, 4}, {false, nbt_house_2,      e_up_idle, {4,-1}} }  // nbt house 2
  };

  const Warp_point *get_warp_table(Map_ID_enum mID);
}