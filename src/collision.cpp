#include "collision.h"
#include <iostream>

// This file implements functions from collision.h

namespace Collision_Data
{
  const GLubyte * get_ptr(Map_ID_enum mID)
  {
    Debug::log_from(Debug::collision,"fetching collision ptr for map id ",+mID, " ", to_str[mID]);
    switch (mID)
    {
      case null_map_id:       return nullptr;
      case new_bark_town:     return new_bark_town_collision_data;
      case player_house_fl1:  return player_house_fl1_collision_data;
      case player_house_fl2:  return player_house_fl2_collision_data;
      case elms_lab:          return elms_lab_collision_data;
      case elms_house:        return elms_house_collision_data;
      case new_bark_house_1:  return new_bark_house_1_collision_data;
      case route_29:          return route_29_collision_data;
      case cherry_grove_city: return cherry_grove_city_collision_data;
      case route_30:          return route_30_collision_data;
      default:
        Debug::log_error("[Collsion] ","error: unhandled map enum; recieved map id ",+mID);
        return nullptr;
    }
  };
}