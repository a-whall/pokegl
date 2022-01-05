#include "game.h"

namespace Game
{
  //Map    * p_active_map= nullptr;
  //Player * p_player= nullptr;
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
    scene_manager.init_maps(*map_shader);
    scene_manager.new_player(*player_shader);
  }

  void Instance::on_update(float t) { /* Empty function */ }

  void Instance::on_exit()
  {
    delete world;
  }
}