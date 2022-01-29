#include "collision.h"
#include <iostream>
#include "texture.h"



using namespace Debug;




const GLubyte * Collision_Data::get_ptr(Map_ID_enum mID)
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
      log_error_from(collision,"no case of get ptr for ",map_str[mID]);
      return nullptr;
  }
}



// This function accesses a bit from one of the constant collision data arrays.
bool Collision_Data::allows_move_to(Point dest, Map_ID_enum mID)
{
  Point max = Texture_Data::get_bounds(mID);
  const GLubyte * p_collision = Collision_Data::get_ptr(mID);
  bool move_allowed= 0 <= dest.x() && dest.x() < max.x() && 0 <= dest.y() && dest.y() < max.y(); // map bounding box check
  int i= max.y() * dest.x() + dest.y(); // index into collision bits array
  if (move_allowed) move_allowed= (p_collision[i/8] >> (7 - (i%8))) & 1; // access collision data iff map bound check passed
  log_from(collision,"move to (",dest.x(),",",dest.y(),"): ",move_allowed);
  return move_allowed;
}