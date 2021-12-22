#include "warps.h"

namespace Warp
{
  using std::unordered_map;
  const Warp_point * get_warp_table(Map_ID_enum mID)
  {
    switch(mID)
    {
      case player_house_fl1:  return player_house_fl1_warp_data;
      case player_house_fl2:  return player_house_fl2_warp_data;
      case new_bark_town:     return new_bark_town_warp_data;
      case route_29_nbt:
      case route_29_cgc:
      case cherry_grove_city: return player_house_fl1_warp_data;
    }
  }
}