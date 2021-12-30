#pragma once
#include <cstdint>
// This file contains the definition of the Map_ID_enum,
// used to identify:
//    - paths to assets
//    - collision data
//    - world nodes
//    - TODO: maintain this list of dependents
// Each enum value is 1 byte therefore imposing a limit of 256
// on the number of maps and all things listed above
enum Map_ID : std::uint8_t
{
  null_map_id,
  // New Bark Town
  new_bark_town,
  player_house_fl1,
  player_house_fl2,
  elms_lab,
  elms_house,
  new_bark_house_1,
  // Route 29
  route_29,
  // Cherry Grove City
  cherry_grove_city,
  cherry_grove_house_1,
  cherry_grove_house_2,
  cherry_grove_house_3,
  cherry_grove_mart,
  cherry_grove_pc,
  // Route 30
  route_30,
  // Route 31
  route_31,
  // Violet City
  violet_city,
  violet_gym,
  sprout_tower,
  violet_school,
  violet_house_1,
  violet_house_2,
  violet_mart,
  violet_pc,
  // Route 32
  route_32,
  // Ruins of Alph
  ruins_of_alph,
  // Union Cave
  union_cave,
  // Route 33
  route_33,
  // Azalea Town
  azalea_town,
  azalea_gym,
  charcoal_kiln,
  slowpoke_well,
  kurts_house,
  azalea_mart,
  azalea_pc,
  // Ilex Forest
  ilex_forest,
  // Route 34
  route_34,
  daycare,
  // Goldenrod City
  goldenrod_city,
  goldenrod_dept_store,
  goldenrod_game_corner,
  goldenrod_radio_tower,
  goldenrod_tunnel,
  goldenrod_pc,
  // Route 35
  route_35,
  // Route 36
  route_36,
  // Route 37
  route_37,
  // Ecruteak City
  ecruteak_city,
  ecruteak_bell_tower,
  ecruteak_burned_tower,
  ecruteak_dance_theatre,
  ecruteak_gym,
  ecruteak_mart,
  ecruteak_pc,
  ecruteak_house_1,
  // Route 38
  route_38,
  // Farm
  farm,
  // Route 39
  route_39,
  // Olivine City
  olivine_city,
  olivine_gym,
  olivine_lighthouse,
  olivine_house_1,
  olivine_house_2,
  olivine_house_3,
  olivine_house_4,
  olivine_ss_aqua,
  olivine_mart,
  olivine_pc,
  // Route 40
  route_40,
  // Route 41
  route_41,
  // Cianwood City
  cianwood_city,
  cianwood_gym,
  cianwood_pc,
  cianwood_house_1,
  cianwood_house_2,
  cianwood_house_3,
  cianwood_house_4,
  cianwood_pharmacy,
  // Route 42
  route_42,
  // Mahogany Town
  mahogany_town,
  mahogany_gym,
  mahogany_mart,
  mahogany_pc,
  mahogany_house,
  // Route 43
  route_43,
  // Lake of Rage
  lake_of_rage,
  // Route 44
  route_44,
  // Blackthorn City
  blackthorn_city,
  blackthorn_gym,
  blackthorn_mart,
  blackthorn_pc,
  blackthorn_house_1,
  blackthorn_house_2,
  blackthorn_house_3,
  // Route 45
  route_45,
  // Route 46
  route_46,
  // Route 27
  route_27,
  max_map_id // <= 255 due to 8 bit type
};

typedef  Map_ID  Map_ID_enum;

// TODO: move this to a better file
typedef struct X_Y_pair { int x, y; } Point, SpawnPoint;

constexpr const char * to_str[max_map_id]=
{
  "null_map_id",
  // New Bark Town
  "new_bark_town",
  "player_house_fl1",
  "player_house_fl2",
  "elms_lab",
  "elms_house",
  "new_bark_house_1",
  // Route 29
  "route_29",
  // Cherry Grove City
  "cherry_grove_city",
  "cherry_grove_house_1",
  "cherry_grove_house_2",
  "cherry_grove_house_3",
  "cherry_grove_mart",
  "cherry_grove_pc",
  // Route 30"
  "route_30",
  // Route 31
  "route_31",
  // Violet City
  "violet_city",
  "violet_gym",
  "sprout_tower",
  "violet_school",
  "violet_house_1",
  "violet_house_2",
  "violet_mart",
  "violet_pc",
  // Route 32
  "route_32",
  // Ruins of Alph
  "ruins_of_alph",
  // Union Cave
  "union_cave",
  // Route 33
  "route_33",
  // Azalea Town
  "azalea_town",
  "azalea_gym",
  "charcoal_kiln",
  "slowpoke_well",
  "kurts_house",
  "azalea_mart",
  "azalea_pc",
  // Ilex Forest
  "ilex_forest",
  // Route 34
  "route_34",
  "daycare",
  // Goldenrod City
  "goldenrod_city",
  "goldenrod_dept_store",
  "goldenrod_game_corner",
  "goldenrod_radio_tower",
  "goldenrod_tunnel",
  "goldenrod_pc",
  // Route 35
  "route_35",
  // Route 36
  "route_36",
  // Route 37
  "route_37",
  // Ecruteak City
  "ecruteak_city",
  "ecruteak_bell_tower",
  "ecruteak_burned_tower",
  "ecruteak_dance_theatre",
  "ecruteak_gym",
  "ecruteak_mart",
  "ecruteak_pc",
  "ecruteak_house_1",
  // Route 38
  "route_38",
  // Farm
  "farm",
  // Route 39
  "route_39",
  // Olivine City
  "olivine_city",
  "olivine_gym",
  "olivine_lighthouse",
  "olivine_house_1",
  "olivine_house_2",
  "olivine_house_3",
  "olivine_house_4",
  "olivine_ss_aqua",
  "olivine_mart",
  "olivine_pc",
  // Route 40
  "route_40",
  // Route 41
  "route_41",
  // Cianwood City
  "cianwood_city",
  "cianwood_gym",
  "cianwood_pc",
  "cianwood_house_1",
  "cianwood_house_2",
  "cianwood_house_3",
  "cianwood_house_4",
  "cianwood_pharmacy",
  // Route 42
  "route_42",
  // Mahogany Town
  "mahogany_town",
  "mahogany_gym",
  "mahogany_mart",
  "mahogany_pc",
  "mahogany_house",
  // Route 43
  "route_43",
  // Lake of Rage
  "lake_of_rage",
  // Route 44
  "route_44",
  // Blackthorn City
  "blackthorn_city",
  "blackthorn_gym",
  "blackthorn_mart",
  "blackthorn_pc",
  "blackthorn_house_1",
  "blackthorn_house_2",
  "blackthorn_house_3",
  // Route 45
  "route_45",
  // Route 46
  "route_46",
  // Route 27
  "route_27"
};