#include "game.h"

namespace Game
{
  typedef struct peripheral_maps
  {
    Map *p_left_map;
    Map *p_right_map;
    Map *p_up_map;
    Map *p_down_map;
    Map *p_up_right_map;
    Map *p_up_left_map;
    Map *p_down_right_map;
    Map *p_down_left_map;
  } Chunks;

  World_node *p_world= nullptr;
  Map    *p_active_map= nullptr;
  Player *p_player= nullptr;
  Shader *map_shader= nullptr, *player_shader= nullptr;
  const Warp::Warp_point * current_warp_list;
  Chunks ov= {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr}; 

  Instance::Instance()
  : Application()
  {
    this->in_overworld= false;
  }

  void Instance::on_init()
  {
    set_music("assets/audio/10 New Bark Town.mp3");

    // Create Shaders
    map_shader    = &scene_manager.new_shader("shader/MapSprite.glsl");
    player_shader = &scene_manager.new_shader("shader/PlayerSprite.glsl");
    
    current_warp_list = Warp::get_warp_table(player_house_fl1);

    p_world = load_world(new_bark_town);

    // Create maps
    ov.p_left_map = &scene_manager.new_map(route_29_nbt, *scene_manager.camera_controller, *map_shader);
    ov.p_left_map->shift_vertex_coords_by_offset(-ov.p_left_map->w_tiles, 0);
    ov.p_left_map->is_visible= false;

    p_active_map = &scene_manager.new_map(player_house_fl1, *scene_manager.camera_controller, *map_shader);
    
    // Create overworld sprites
    p_player = &scene_manager.new_sprite<Player>(*scene_manager.camera_controller, &p_active_map, *player_shader);
  }

  // prototype
  void Instance::on_update(float t)
  {
    auto do_warp = [](Warp::Dest dst) {
      p_active_map->change(dst.mID);
      p_player->set_position(dst.spawn_point.x, dst.spawn_point.y);
    };

    for (int i = 0; i < Warp::num_warps[p_active_map->current_mID]; ++i)
    {
      if (p_player->x_position == current_warp_list[i].p.x && p_player->y_position == current_warp_list[i].p.y)
        do_warp(current_warp_list[i].dst);
    }

    // if (!in_overworld)
    // // TODO: FINISH MORE MAPS AND GET THEM INTO THE ASSETS FOLDER
    // // TODO: START TO IMPLEMENT THE WARP POINTS CHECKS BASED ON MAP.
    // // TODO: PROTOTYPE A SYSTEM TO IMPLEMENT WHICH MAPS ARE CONNECTED TO EACH OTHER (some sort of graph would probably work nice)
    // {//======= if not in the overworld =================================================================
      
    //   if (p_active_map->current_mID == player_house_fl1) {
    //     if (p_player->x_position == 9 && p_player->y_position == 7)
    //     {
    //       p_active_map->change(player_house_fl2);
    //       p_player->set_position(7, 4);
    //     }
    //     else if (p_player->x_position == 7 && p_player->y_position == 0)
    //     {
    //       p_active_map->change(new_bark_town);
    //       p_player->set_position(23, 11);
    //       in_overworld = true;
    //       ov.p_left_map->is_visible = true;
    //     }
    //   }
    //   else if (p_active_map->current_mID == player_house_fl2) {
    //     if (p_player->x_position == 7 && p_player->y_position == 5) {
    //       p_active_map->change(player_house_fl1);
    //       p_player->set_position(9, 6);
    //     }
    //   }
    // }
    // else {//============= in overworld ==========================================================
    //   if (p_active_map->current_mID == new_bark_town) {
    //     if (p_player->x_position == 23 && p_player->y_position == 12) {
    //       p_active_map->change(player_house_fl1);
    //       p_player->set_position(7, 1);
    //       in_overworld = false;
    //       ov.p_left_map->is_visible = false;
    //     }
    //     if (p_player->x_position == 0 && p_player->y_position < 12 && p_player->y_position > 3) {
    //       p_active_map->change(route_29_nbt);
    //       p_player->set_position(31, p_player->y_position);
    //     }
    //   }
    // }
  }

  void try_move_to(int x, int y)
  {

  }


  void draw_overworld() {

  }
}