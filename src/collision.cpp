#include "collision.h"
#include <iostream>

// This file implements functions from collision.h

namespace Collision_Data
{
  std::unordered_map< Map_ID_enum, Point > bounds;

  const GLubyte * get_ptr(Map_ID_enum mID)
  {
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
        Debug::log_error("[Collsion] error: no case of get ptr for ",to_str[mID]);
        return nullptr;
    }
  }

  // Attempts to associate boundaries to the given map ID. It will only modify the bounds hashmap if an element has not already been specified for mID. That way map IDs will only be able to associate with one set of boundaries for the duration of the program. If one has already been set the function simply returns. The intention here is to automatically register bounds based on map width and height when the map texture is loaded rather than hard coding that data.
  // @param mID: used as a key in the global (to Collision_Data namespace) bounds hashmap.
  // @param w_tiles,h_tiles: becomes the upper bounds for when collision data of mID is accessed.
  void try_to_def_bounds(Map_ID_enum mID, int w_tiles, int h_tiles)
  {
    if (bounds.find(mID) == bounds.end())
       bounds[mID]= { w_tiles, h_tiles };
  }

  Point get_bounds(Map_ID_enum mID)
  {
    if (bounds.find(mID) == bounds.end())
      Debug::log_error_abort("[Collision] fatal error: boundaries for ",to_str[mID]," undefined");
    return bounds[mID]; 
  }

  // This function accesses a bit from one of the constant collision data arrays.
  bool allows_move_to(Point dest, Map_ID_enum mID)
  {
    Point max = get_bounds(mID);
    const GLubyte * p_collision = Collision_Data::get_ptr(mID);
    bool move_allowed= 0 <= dest.x && dest.x < max.x && 0 <= dest.y && dest.y < max.y; // map bounds checking
    int i= max.y * dest.x + dest.y; // index into collision bits array
    if (move_allowed) move_allowed= (p_collision[i/8] >> (7 - (i%8))) & 1; // access collision data iff map bound check passed
    Debug::log_from(Debug::collision,"move to (",dest.x,",",dest.y,"): ",move_allowed);
    return move_allowed;
  }
}