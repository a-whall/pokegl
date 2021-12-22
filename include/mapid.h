#pragma once

typedef enum
{ // for all existing maps
  player_house_fl1,
  player_house_fl2,
  new_bark_town,
  route_29_nbt,
  route_29_cgc,
  cherry_grove_city,
  route_27_nbt,
  elms_lab,
  nbt_house_1,
  nbt_house_2
} Map_ID_enum;

typedef struct X_Y_pair { int x, y; } Point, SpawnPoint;