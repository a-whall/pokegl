#pragma once
#include "sprite.h"
#include "map.h"
#include "animation.h"
#include "warps.h"
#include "world.h"

class Player
: public Sprite
{
  GLuint vao,  vbo,  ebo,  opengl_texture_ID;
  Animation::FSM fsm;
  World_Graph * p_world_graph;
  int frame_prevent_interupt_counter;
  int stride_left;
  const Warp::Destination * pending_warp = nullptr;
public:
  Map * * maps;
  int x_position, y_position;

  Player(Camera&, World_Graph*, Shader&);
  ~Player();
  void init_animations();
  void init_buffers() override;
  void update(float, const Uint8 *) override;
  void render() override;
  void set_position(int, int);
  void set_position(const Warp::Destination&); // maybe use Point as arg instead
  void start_animation();
private:
  void do_warp(const Warp::Destination&);
  void take_input(const Uint8 *);
  void update_frame_to_stride();
  void update_frame_to_idle();
  //void change_position(float, float);
  void update_position();
  void walk_anim_end();
  void walk_animation(int *);
};