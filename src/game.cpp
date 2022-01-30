#include "game.h"



void PokeGL::on_init()
{
  scene_manager.sound.load_sfx();
  Shader * player_shader= &scene_manager.new_shader("shader/PlayerSprite.glsl");
  scene_manager.init_buffers();
  scene_manager.init_text();
 (scene_manager.world_graph=new World_Graph())->set_current_node(player_house_fl1);
  scene_manager.init_maps();
  scene_manager.new_player(*player_shader);
  Mix_VolumeMusic(16);
  Mix_Volume(-1, 32);
  scene_manager.sound.play_song(new_bark_town_mp3);
}



void PokeGL::on_update(float t)
{
// empty
}



void PokeGL::on_exit()
{
// empty
}