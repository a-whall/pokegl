#pragma once
#include <cstdint>

enum SFX_ID : std::uint8_t
{
  door_in,
  door_out,
  bump_wall,
  ledge_hop,
  max_sfx_id
};

typedef SFX_ID SFX_ID_enum;