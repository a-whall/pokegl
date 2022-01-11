#include "game.h"



void PokeGL::on_init()
{
  scene_manager.sound.load_sfx();
  Shader * player_shader= &scene_manager.new_shader("shader/PlayerSprite.glsl");
  scene_manager.init_text();
  scene_manager.world_graph= world= new World_Graph();;
  world->set_current_node(player_house_fl1);
  scene_manager.init_maps();
  scene_manager.new_player(*player_shader);
  scene_manager.sound.play_song(new_bark_town_mp3);
}



void PokeGL::on_update(float t)
{
  /* Empty function */
}



void PokeGL::on_exit()
{
  delete world;
}