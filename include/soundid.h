#pragma once
#include <cstdint>

enum Track_ID : std::uint32_t
{
  null_track,
  title_screen_mp3,
  an_adventure_begins_mp3,
  new_bark_town_mp3,
  hurry_along_mp3,
  elms_lab_mp3,
  route_29_mp3,
  battle_day_mp3,
  battle_night_mp3,
  battle_victory_mp3,
  cherry_grove_city_mp3,
  hurry_along_2_mp3,
  pokemon_center_mp3,
  route_30_mp3,
  professor_oak_mp3,
  a_rival_appears_mp3,
  a_trainer_appears_boy,
  battle_trainer_mp3,
  violet_city_mp3,
  sprout_tower_mp3,
  a_trainer_appears_sage_mp3,
  evolution_mp3,
  gym_mp3,
  battle_gym_leader,
  battle_gym_leader_victory_mp3,
  route_32_mp3,
  ruins_of_alph_grounds_mp3,
  ruins_of_alph_mp3,
  a_trainer_appears_girl_mp3,
  a_trainer_appears_sus2_mp3,
  a_trainer_appears_sus1_mp3,
  azalea_town_mp3,
  slowpoke_well_mp3,
  a_trainer_appears_rocket_mp3,
  battle_rocket_mp3,
  a_trainer_appears_boy2_mp3,
  goldenrod_city_mp3,
  goldenrod_game_corner_mp3,
  pokegear_oaks_talk_mp3,
  pokegear_channel_mp3,
  pokegear_march_mp3,
  pokegear_lullaby_mp3,
  pokegear_buenas_password_mp3,
  pokegear_unown,
  cycling_mp3,
  a_trainer_appears_girl1_mp3,
  national_park_mp3,
  bug_contest_begin_mp3,
  bug_contest_mp3,
  ecruteak_city_mp3,
  ecruteak_dance_theater_mp3,
  a_trainer_appears_kimono_girl_mp3,
  burned_tower_mp3,
  eusine_mp3,
  route_38_mp3,
  battle_legendary_beast_mp3,
  olivine_lighthouse_mp3,
  surf_mp3,
  battle_tower_reception_desk_mp3,
  battle_tower_mp3,
  route_42_mp3,
  radio_transmission_mp3,
  team_rocket_hq_mp3,
  radio_tower_occupied_mp3,
  dragons_den_mp3,
  clair_mp3,
  route_26_mp3,
  victory_road_mp3,
  pokemon_leage_mp3,
  battle_champion_mp3,
  hall_of_fame_mp3,
  ss_aqua_mp3,
  vermillion_city_mp3,
  battle_kanto_trainer_mp3,
  battle_kanto_gym_leader_mp3,
  route_6_mp3,
  battle_wild_kanto,
  saffron_city_mp3,
  lavender_town_mp3,
  rock_tunnel_mp3,
  celadon_city_mp3,
  route_15_mp3,
  pokegear_pokeflute_mp3,
  viridian_forest_mp3,
  mt_moon_mp3,
  pallet_town_mp3,
  tin_tower_mp3,
  ending_theme_mp3,
  the_end_mp3,
};

enum SFX_ID : std::uint8_t
{
  door_in,
  door_out,
  bump_wall,
  ledge_hop,
  max_sfx_id
};

typedef  SFX_ID  SFX_ID_enum;
typedef  Track_ID  Track_ID_enum;