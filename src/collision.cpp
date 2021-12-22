#include "collision.h"
#include <iostream>

// This file implements functions from collision.h

namespace Collision
{
  const GLubyte* load(Map_ID_enum mID)
  {
    Debug::log("[Collision] ","loading mapID ",mID);
    switch (mID)
    {
      case player_house_fl1:  return player_house_fl1_collision_data;
      case player_house_fl2:  return player_house_fl2_collision_data;
      case new_bark_town:     return new_bark_town_collision_data;
      case route_29_nbt:      return route_29_nbt_collision_data;
      case route_29_cgc:      return route_29_cgc_collision_data;
      case cherry_grove_city: return cherry_grove_city_collision_data;
      default:                Debug::log_error_abort("[Collsion] ", "::load() error: unhandled map enum");
    }
  };
}