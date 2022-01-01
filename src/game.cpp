#include "game.h"

namespace Game
{
  Map    * p_active_map= nullptr;
  Player * p_player= nullptr;
  Shader * map_shader= nullptr;
  Shader * player_shader= nullptr;

  void Instance::on_init()
  {
    scene_manager.sound.load_sfx();
    scene_manager.sound.play_song(new_bark_town);
    map_shader= &scene_manager.new_shader("shader/MapSprite.glsl");
    player_shader= &scene_manager.new_shader("shader/PlayerSprite.glsl");
    scene_manager.world_graph= world= new World_Graph();;
    world->set_current_node(player_house_fl1);
    p_active_map = &scene_manager.init_map(*map_shader);
    p_player = &scene_manager.new_player(*player_shader);
    //overworld_chunks.p_left_map = &scene_manager.new_map(route_29, *map_shader);
    //overworld_chunks.p_left_map->shift_vertex_coords_by_offset(-overworld_chunks.p_left_map->w_tiles, 0);
    //overworld_chunks.p_left_map->is_visible= false;
  }

  void Instance::on_update(float t) { /* Empty function */ }

  void Instance::on_exit()
  {
    delete world;
  }
}