#include "player.h"
#include <gtc/matrix_transform.hpp>
#include "camera.h"
#include "frameid.h"
#include "collision.h"
#include "warps.h"
#include "scene.h"
#include "texture.h"
#include <gtx/string_cast.hpp>



using namespace Debug;



// Player constructor : Initialize a bunch of player state values. Starts the player facing down.
// @param cam: a reference to the scene camera
// @param active_map: a pointer to a pointer which points to the active map (the central map tile of the scene)
// @param s: a reference to shader for sprite animation
Player::Player(Scene::Manager& man, Shader &s)
: Sprite(.25f, .5f, 0.001f, *man.camera_controller, s),
  scene_manager(man),
  fsm()
{
  obj_identify(player,alloc,this,"Player");

  // initialize all member variables to 0
  frame_prevent_interupt_counter
  = info_button_delay_counter
  = bump_counter 
  = stride_left
  = x_position
  = y_position = 0;

  fsm.set_shader(&shader);
  init_buffers();
  init_animations();
  glUseProgram(shader.handle);
  shader.set("player_frame_ID", idle_d);
  glUseProgram(0);
}



// Player destructor : Delete all OpenGL objects associated with this player sprite
Player::~Player()
{
  glDeleteBuffers(1, &vb);
  glDeleteBuffers(1, &eb);
  glDeleteVertexArrays(1, &va);
  glDeleteTextures(1, &t);
  obj_identify(player,dealloc,this,"Player");
}



// Frames are loaded using IMG_Load() from the SDL_image library. This function creates an OpenGL texture2D_array texture object which is intended to be cleaned up by the player destructor.
// @warning: the IMG_INIT_PNG flag must be passed to the SDL function IMG_INIT in order to load the png files used for these animation frames.
void Player::init_animations()
{
  // note: order matches Frame_ID enum
  std::vector<const char*> frames=
  {
    "assets/sprite/player/backward_idle.png",
    "assets/sprite/player/left_idle.png",
    "assets/sprite/player/forward_idle.png",
    "assets/sprite/player/right_idle.png",
    "assets/sprite/player/backward_stride1.png",
    "assets/sprite/player/backward_stride2.png",
    "assets/sprite/player/left_stride.png",
    "assets/sprite/player/forward_stride1.png",
    "assets/sprite/player/forward_stride2.png",
    "assets/sprite/player/right_stride.png"
  };

  // Initialize OpenGL texture object
  generate_texture(GL_TEXTURE_2D_ARRAY, &t);

  // Upload animation frame data to GPU
  int w, h, bytes_per_pixel= 4, count= frames.size();
  GLubyte * image_data= load_textures(frames, player, this, &w, &h, bytes_per_pixel);
  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, w, h, count); // allocate GPU memory
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, w, h, count, GL_RGBA, GL_UNSIGNED_BYTE, image_data); // send pixel data to GPU
  delete image_data; // free pixel data from heap

  // Initialize the frame state model and their associations
  fsm
   .create_state("player_idle_down"   , idle_d,    { idle_l, idle_r, idle_u, stride_d2, stride_d1 })
   .create_state("player_idle_up"     , idle_u,    { idle_l, idle_r, idle_d, stride_u1, stride_u2 })
   .create_state("player_idle_right"  , idle_r,    { idle_l, idle_u, idle_d, stride_r1 })
   .create_state("player_idle_left"   , idle_l,    { idle_r, idle_u, idle_d, stride_l1 })
   .create_state("player_stride_left" , stride_l1, { idle_l })
   .create_state("player_stride_right", stride_r1, { idle_r })
   .create_state("player_stride_down1", stride_d1, { idle_d })
   .create_state("player_stride_down2", stride_d2, { idle_d })
   .create_state("player_stride_up1"  , stride_u1, { idle_u })
   .create_state("player_stride_up2"  , stride_u2, { idle_u });
}



// This function declares VBO and EBO data temporarily on stack and sends it to the GPU. VBO and EBO data will not change for the duration of the program. VBO data represents model coordinates of the player texture, i.e. a 1x1 quad centered on origin, as well as texture coordinates for texture mapping which is handled by OpenGL. EBO data represents the order in which vertices in the VBO are to be drawn to make triangles by the OpenGL draw calls.
void Player::init_buffers()
{
  float vb_data[]=
  {
    -.5f, +.5f, 0.f,       0.f, 0.f,
    +.5f, +.5f, 0.f,       1.f, 0.f,
    +.5f, -.5f, 0.f,       1.f, 1.f,
    -.5f, -.5f, 0.f,       0.f, 1.f
  };
  unsigned eb_data[]=
  {
    0, 1, 2,
    2, 3, 0
  };
  n_verts= 6;
  glGenVertexArrays(1, &va);
  glGenBuffers(1, &eb);
  glGenBuffers(1, &vb);
  log_from(player,"specifying buffers");
  glBindVertexArray(va);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(eb_data), eb_data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vb_data), vb_data, GL_STATIC_DRAW);
  glEnableVertexArrayAttrib(va, 0);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 20, (void*)0);
  glEnableVertexArrayAttrib(va, 1);
  glVertexAttribPointer(1, 2, GL_FLOAT, false, 20, (void*)12);
  glBindVertexArray(0);
}



// Indefinitely updates the players model-view matrix using the camera's view matrix. Then checks whether to continue executing an animation phase or to accept and process input.
// @param t: the intended duration of 1 frame of the application
// @param key_states: a pointer to SDL pressed keys data
void Player::update(float t, const Uint8* key_states)
{
  mv= cam.view() * model;
//   if (first) {
//     log_from(player,"M ",glm::to_string(model));
//     log_from(player,"V ",glm::to_string(cam->view));
//     log_from(player,"MV ",glm::to_string(mv));
//   }
  
  //log_from(player,"V ",glm::to_string(cam.view()));

  if (frame_prevent_interupt_counter) {
    // TODO: more general continue_animation() function, along with a higher level animation-state value to decide which animation to 'continue'
    walk_animation(&frame_prevent_interupt_counter);
  }
  else {
    take_input(key_states);
    World_Node *  wnode= scene_manager.world_graph->get_current_node();
    Map_ID_enum   dest_mID= wnode->mID[0];
    Frame_ID_enum fID= fsm.get_fid();
    Point dest= {x_position,y_position};

    if      (key_states[SDL_SCANCODE_W]) { ++dest.y(); fID=idle_u; }
    else if (key_states[SDL_SCANCODE_A]) { --dest.x(); fID=idle_l; }
    else if (key_states[SDL_SCANCODE_S]) { --dest.y(); fID=idle_d; }
    else if (key_states[SDL_SCANCODE_D]) { ++dest.x(); fID=idle_r; }

    if (dest.x() == x_position && dest.y() == y_position) // if no movement
      return;                                            // no need to do anything further

    fsm.input(fID); // may need a "turn_around" animation that takes a couple frames to delay input.

    const Warp::Warp_Point * warps = Warp::get_warp_table(dest_mID);
    for (int i = 0; i < Warp::num_warps[dest_mID]; ++i)         // for each warp point
    {
      if (dest.x() == warps[i].p.x() && dest.y() == warps[i].p.y()) { // if the destination is equal to the current warp point
        log_from(player,"warp point found");       // log a message that warp point has been identified
        if ( !suspend_warp(warps[i].dst) ) {                        // should the player take one final step before the warp takes place?
          do_warp(warps[i].dst);
        }
        return;
      }
    }

    Point spawn = dest;            // this is of use when the player is trying to move to a neighbor map
    if (wnode->in_overworld) {     // in overworld logic: prepare to potentially check neighbor map
      if ( wnode->mID[up] != null_map_id ) {
        if ( dest.y() == Texture_Data::get_bounds(wnode->mID[up]).y() ) {
          dest_mID = wnode->mID[up]; // change the mID to determine which p_collision to retrieve
          dest.y() = 0;                // "overflow" destination coordinate
          spawn.y() = -1;              // update the location the player will need to be placed for the impending map update
        }
      }
      if ( wnode->mID[up] != null_map_id ) {
        if ( dest.x() == -1 ) {
          dest_mID = wnode->mID[left];
          dest.x() = Texture_Data::get_bounds(dest_mID).x() - 1; // 1 to the left of position
          spawn.x() = dest.x() + 1;                              // 1 to the right of destination
        }
      }
      if ( wnode->mID[up] != null_map_id ) {
        if ( dest.y() == -1 ) {
          dest_mID = wnode->mID[down];
          dest.y() = Texture_Data::get_bounds(dest_mID).x() - 1;
          spawn.y() = dest.y() + 1;
        }
      }
      if ( wnode->mID[up] != null_map_id ) {
        if ( dest.x() == Texture_Data::get_bounds(wnode->mID[right]).x() ) {
          dest_mID = wnode->mID[right];
          dest.x() = 0;
          spawn.x() = -1;
        }
      }
    }
    if ( Collision_Data::allows_move_to(dest, dest_mID) ) {
      if (fsm == fID && frame_prevent_interupt_counter == 0) {
        start_animation();
        if (dest_mID != wnode->mID[0]) {      // if the destination is not the current map
          scene_manager.sync_world(dest_mID);    // let the scene manager sync the world based on a destination Map_ID
          set_position(spawn);
        }
      }
    }
    else {
      if (bump_counter == 0) {
        scene_manager.sound.play_sfx(bump_wall);
        bump_counter= 16;
      }
    }
    if (bump_counter > 0) {
      --bump_counter;
    }
  }
}



// Sends updated matrix data to the PlayerSprite shader and issues an OpenGL draw call.
void Player::render()
{
  glUseProgram(shader.handle);
  shader.set("player_mvp", cam.projection() * mv);
//   if (first) {
//     first=false;
//     log_from(player,"P ",glm::to_string(cam->proj));
//     mat4 mvp (cam->proj * mv);
//     log_from(player,"MVP ",glm::to_string(mvp));
//     glm::vec4 ndc (mvp * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f));
//     log_from(player,"MVP * v[0] ",glm::to_string(ndc));
//     ndc.x /= ndc.w;
//     ndc.y /= ndc.w;
//     ndc.z /= ndc.w;
//     log_from(player,"normalized device coordinates: ",glm::to_string(ndc));
//   }
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, t);
  glBindVertexArray(va);
  glDrawElements(GL_TRIANGLES, n_verts, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}



// Shift the position of the player as well as the camera
void Player::set_position(Point p)
{
  float x= p.x() + 0.25f;
  float y= p.y() + 0.50f;
  x_position= p.x();
  y_position= p.y();
  vec3 xyCoordinates= vec3(x, y, .001f);
  model= translate(mat4(1.0f), xyCoordinates);
  cam.set_Position(vec3(x, y, cam.get_position().z));
}



void Player::set_position(const Warp::Destination& dest)
{
  this->set_position(dest.spawn_point);
}



// Set the frame counter appropriately
void Player::start_animation()
{
  frame_prevent_interupt_counter= 16;
}



bool Player::suspend_warp(const Warp::Destination& dest)
{
  if (dest.step_before) {
    start_animation();
    pending_warp= &dest;
    return true;
  }
  return false;
}



// This function is called preciesly when a warp should happen.
void Player::do_warp(const Warp::Destination& dest)
{
  log_from(player,"performing warp");
  fsm.input(dest.fID);                // input the facing direction
  scene_manager.sync_world(dest.mID); // sets current world node and changes maps array
  this->set_position(dest);           // update the player position
  if (dest.step_after)
    start_animation();
}



// Use this function to process any extra key press logic for player
void Player::take_input(const Uint8* key_states)
{
  if (info_button_delay_counter == 0) {
    if (key_states[SDL_SCANCODE_P]) std::cout << "[Player] position = " << x_position << ',' << y_position << '\n';
    if (key_states[SDL_SCANCODE_F]) std::cout << "[Player] current frame state: " << fsm.get_fid() << '\n';
    info_button_delay_counter= 8;
  }
  if (info_button_delay_counter > 0) --info_button_delay_counter;
}



void Player::update_frame_to_stride()
{
  if      (fsm == idle_l) fsm.input(stride_l1);
  else if (fsm == idle_r) fsm.input(stride_r1);
  else if (fsm == idle_d) fsm.input((stride_left= !stride_left) ? stride_d1 : stride_d2);
  else if (fsm == idle_u) fsm.input((stride_left= !stride_left) ? stride_u1 : stride_u2);
  else log_error_from(player,"failed to update frame to stride");
}



void Player::update_frame_to_idle()
{
  if      (fsm == stride_l1)                     fsm.input(idle_l);
  else if (fsm == stride_r1)                     fsm.input(idle_r);
  else if (fsm == stride_d1 || fsm == stride_d2) fsm.input(idle_d);
  else if (fsm == stride_u1 || fsm == stride_u2) fsm.input(idle_u);
  else log_error_from(player,"failed to update frame to idle");
}



void Player::update_position()
{
  auto change_position = [&](float dx, float dy) {
    vec3 x_y_translation= vec3(dx, dy, 0.0f);  // Create a vector with the given offsets
    model = translate(model, x_y_translation); // Translate the model matrix of the player with the vector above
    cam.move(x_y_translation);                 // Apply that same translation to the games camera
  };
  float delta_pos= 0.0625f; // 1/16th because 16 frames total in the walk animation --> the player will move exactly 1 unit total per step taken
  if      (fsm == idle_l || fsm == stride_l1) change_position(-delta_pos, 0.0f);
  else if (fsm == idle_r || fsm == stride_r1) change_position(delta_pos, 0.0f);
  else if (fsm == idle_u || fsm == stride_u1 || fsm == stride_u2) change_position(0.0f, delta_pos);
  else if (fsm == idle_d || fsm == stride_d1 || fsm == stride_d2) change_position(0.0f, -delta_pos);
  else log_error_from(player,"failed to update position");
}



void Player::walk_anim_end()
{
  // update position coordinates
  if      (fsm == idle_u) ++y_position;
  else if (fsm == idle_l) --x_position;
  else if (fsm == idle_d) --y_position;
  else if (fsm == idle_r) ++x_position;
  else log_error_from(player,"failed to update coordinates, animation state may be incorrect");
  // handle pending warps
  if (pending_warp != nullptr) {
    do_warp(*pending_warp);
    if (pending_warp->step_after)
      start_animation();
    pending_warp = nullptr;
  }
}



// Updates frames at specific frame counts over a 16 frame interval. Frame counter decrements invariably, therefore this function shouldn't be called unless *frame_counter is greater than zero. 4 frames are spent in idle pose, 8 frames in stride pose, then 4 frames are spent back in idle so if you keep walking there is an equal frame interval. Assumes 60 FPS.
void Player::walk_animation(int *frame_counter)
{
  if     (*frame_counter < 1)   log_error_abort(player,"walk_animation frame_counter should be at least 1 and was not");
  if     (*frame_counter == 4)  update_frame_to_idle();
  else if(*frame_counter == 12) update_frame_to_stride();
  update_position();
  (*frame_counter)--;
  if (*frame_counter == 0) walk_anim_end();
}