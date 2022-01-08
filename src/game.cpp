#include "game.h"

namespace Game
{
  Shader * map_shader= nullptr;
  Shader * player_shader= nullptr;
  Shader * text_shader= nullptr;
  Shader * text_box_shader= nullptr;

  void Instance::on_init()
  {
    scene_manager.sound.load_sfx();
    map_shader= &scene_manager.new_shader("shader/MapSprite.glsl");
    player_shader= &scene_manager.new_shader("shader/PlayerSprite.glsl");
    text_shader= &scene_manager.new_shader("shader/Text.glsl");
    text_box_shader= &scene_manager.new_shader("shader/TextBox.glsl");
    scene_manager.init_text(*text_shader, *text_box_shader);
    scene_manager.world_graph= world= new World_Graph();;
    world->set_current_node(player_house_fl1);
    scene_manager.init_maps(*map_shader);
    scene_manager.new_player(*player_shader);
    scene_manager.sound.play_song(new_bark_town);
  }

  void Instance::on_update(float t) { /* Empty function */ }

  void Instance::on_exit()
  {
    delete world;
  }
}