#include "game.h"

namespace Game
{
  Map    * p_active_map= nullptr;
  Player * p_player= nullptr;
  Shader * map_shader= nullptr;
  Shader * player_shader= nullptr;
  const Warp::Warp_Point * current_warp_list;

  Instance::Instance()
  : Application()
  {
    world = new World_Graph();
  }

  Instance::~Instance()
  {
    delete world;
  }

  void Instance::on_init()
  {
    // load sound effects
    //Mix_LoadWAV("assets/audio/sfx/go_inside.wav");
    //Mix_LoadWAV("assets/audio/sfx/go_outside.wav");
    // this function should probably load some state from a save file, but that can be implemented later
    set_music("assets/audio/music/new_bark_town.mp3");
    map_shader= &scene_manager.new_shader("shader/MapSprite.glsl");
    player_shader= &scene_manager.new_shader("shader/PlayerSprite.glsl");
    scene_manager.world_graph = world;
    world->set_current_node(player_house_fl1);
    p_active_map = &scene_manager.init_map(*map_shader);
    //overworld_chunks.p_left_map = &scene_manager.new_map(route_29, *map_shader);
    //overworld_chunks.p_left_map->shift_vertex_coords_by_offset(-overworld_chunks.p_left_map->w_tiles, 0);
    //overworld_chunks.p_left_map->is_visible= false;
    p_player = &scene_manager.new_player(*player_shader);
  }

  void Instance::on_update(float t) { /* Empty function */ }
}