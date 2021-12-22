#pragma once
#include "sprite.h"
#include "scene.h"
#include "map.h"
#include "animation.h"
#include "warps.h"

class Player
: public Sprite
{
  GLuint vao,  vbo,  ebo,  opengl_texture_ID;
  Animation::Automaton player_animation_frame_state;
  Map **pp_active_map;
  int frame_prevent_interupt_counter;
  int stride_left;

  Warp::Dest *pending_warp = nullptr;

public:
  
  int x_position, y_position;

  Player(Camera&, Map **, Shader&);
  ~Player();

  void init_animations();
  void init_buffers() override;
  void update(float, const Uint8 *) override;
  void render() override;
  void set_position(int, int);

private:

  void take_input(const Uint8 *);
  void update_frame_to_stride();
  void update_frame_to_idle();
  void change_position(float, float);
  void update_position();
  void walk_anim_end_update_coordinates();
  void walk_animation(int *);
};