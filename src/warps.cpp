#include "warps.h"
#include "debug.h"

namespace Warp
{
  const Warp_Point * get_warp_table(Map_ID_enum mID)
  {
    switch(mID)
    {
      case new_bark_town:     return new_bark_town_warp_data;
      case player_house_fl1:  return player_house_fl1_warp_data;
      case player_house_fl2:  return player_house_fl2_warp_data;
      case elms_lab:          return elms_lab_warp_data;
      case elms_house:        return elms_house_warp_data;
      case new_bark_house_1:  return new_bark_house_1_warp_data;
      case route_29:          return nullptr;
      case cherry_grove_city: return player_house_fl1_warp_data;
      default: Debug::log_error_abort(Debug::warp,"unhandled map id");
    }
     return nullptr;
  }
}