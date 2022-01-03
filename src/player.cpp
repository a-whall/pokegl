#include "player.h"
#include <SDL_image.h> // IMG_Load
#include <gtc/matrix_transform.hpp>
#include "camera.h"
#include "frameid.h"
#include "collision.h"
#include "warps.h"
#include "scene.h"

// Player constructor : Initialize a bunch of player state values. Starts the player facing down.
// @param cam: a reference to the scene camera
// @param active_map: a pointer to a pointer which points to the active map (the central map tile of the scene)
// @param s: a reference to shader for sprite animation
Player::Player(Scene::Manager& man, Shader &s)
: Sprite(0.25f, 0.5f, .001f, *man.camera_controller, s),
  scene_manager(man),
  fsm()
{
  frame_prevent_interupt_counter = stride_left = x_position = y_position = 0;
  fsm.set_shader(&shader);                                 
  init_buffers();
  init_animations();
  glUseProgram(shader.handle);
  shader.set("frameID", idle_d);
  glUseProgram(0);
}

// Player destructor : Delete all OpenGL objects associated with this player sprite
Player::~Player()
{
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glDeleteVertexArrays(1, &vao);
  glDeleteTextures(1, &opengl_texture_ID);
}

// Frames are loaded using IMG_Load() from the SDL_image library. This function creates an OpenGL texture2D_array texture object which is intended to be cleaned up by the player destructor.
// @warning: the IMG_INIT_PNG flag must be passed to the SDL function IMG_INIT in order to load the png files used for these animation frames.
void Player::init_animations()
{
  typedef struct {
    size_t
      width_pixels=    16,
      height_pixels=   width_pixels,
      count=           10,
      bytes_per_pixel= 4,
      bytes_per_frame= width_pixels * height_pixels * bytes_per_pixel;
  } Frame_Info;

  SDL_Surface * loaded_image= nullptr;
  constexpr Frame_Info f_info;
  GLubyte * p_pixel_data= new GLubyte[f_info.bytes_per_frame * f_info.count];
  unsigned offset= 0;

  std::vector<const char*> frames =
  { // note: order matches Frame_ID enum
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

  glActiveTexture(GL_TEXTURE0); // specify which GL_TEXTURE unit subsequent texture state calls will affect
  glGenTextures(1, &opengl_texture_ID);
  glBindTexture(GL_TEXTURE_2D_ARRAY, opengl_texture_ID);

  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, f_info.width_pixels, f_info.height_pixels, f_info.count); // allocate GPU memory
  for (const char * file_path : frames)
  { // Load each frame's pixel data from file into a GLubyte array
    loaded_image = IMG_Load(file_path);
    if (loaded_image == nullptr)
      Debug::log_error_abort("[Player] Player::init_animations failed to load a texture:", file_path);
    memcpy(p_pixel_data + offset, loaded_image->pixels, f_info.bytes_per_frame);
    SDL_FreeSurface(loaded_image);
    offset += f_info.bytes_per_frame;
  }
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, f_info.width_pixels, f_info.height_pixels, f_info.count, GL_RGBA, GL_UNSIGNED_BYTE, p_pixel_data); // send pixel data to GPU memory
  delete p_pixel_data; // free pixel data from heap

  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  fsm
    .create_state("player_idle_down"   , idle_d,    { idle_l,idle_r,idle_u,stride_d2,stride_d1 })
    .create_state("player_idle_up"     , idle_u,    { idle_l,idle_r,idle_d,stride_u1,stride_u2 })
    .create_state("player_idle_right"  , idle_r,    { idle_l,idle_u,idle_d,stride_r1 })
    .create_state("player_idle_left"   , idle_l,    { idle_r,idle_u,idle_d,stride_l1 })
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
  float vbo_data[]=
  {
   -0.5f, 0.5f, 0.0f,      0.0f, 0.0f,
    0.5f, 0.5f, 0.0f,      1.0f, 0.0f,
    0.5f,-0.5f, 0.0f,      1.0f, 1.0f,
   -0.5f,-0.5f, 0.0f,      0.0f, 1.0f
  };
  unsigned ebo_data[]=
  {
    0, 1, 2,
    2, 3, 0
  };
  this->n_verts= 6;
  glGenVertexArrays(1, &this->vao);
  glGenBuffers(1, &this->ebo);
  glGenBuffers(1, &this->vbo);
  glBindVertexArray(this->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ebo_data), ebo_data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_data), vbo_data, GL_STATIC_DRAW);
  glEnableVertexArrayAttrib(this->vao, 0);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 20, (void*)0);
  glEnableVertexArrayAttrib(this->vao, 1);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 20, (void*)12);
  glBindVertexArray(0);
}

 // Indefinitely updates the players model-view matrix using the camera's view matrix. Then checks whether to continue executing an animation phase or to accept and process input.
 // @param t: the intended duration of 1 frame of the application
 // @param key_states: a pointer to SDL pressed keys data
void Player::update(float t, const Uint8* key_states)
{
  mv= cam.get_WorldToView_Matrix() * model;
  if (frame_prevent_interupt_counter)
    walk_animation(&frame_prevent_interupt_counter);
  else
  {
    take_input(key_states);
    int x_max, y_max;
    const GLubyte * p_collision;
    World_Node * wnode= scene_manager.world_graph->get_current_node();
    Map_ID_enum mID= wnode->mID;
    Frame_ID_enum fID;
    Point dest= {x_position,y_position};

    auto can_move_to = [&](Point dest)
    {
      bool move_allowed=  (0 <= dest.x && dest.x < x_max && 0 <= dest.y && dest.y < y_max); // map bounds checking
      int i= y_max * dest.x + dest.y; // index into collision bits array
      if (move_allowed) {
        move_allowed= (p_collision[i/8] >> (7 - (i%8))) & 1; // access collision data
      }
      Debug::log_from(Debug::player,"move allowed to (",dest.x,",",dest.y,"): ",move_allowed);
      return move_allowed;
    };

    if      (key_states[SDL_SCANCODE_W]) { ++dest.y; fID=idle_u; }
    else if (key_states[SDL_SCANCODE_A]) { --dest.x; fID=idle_l; }
    else if (key_states[SDL_SCANCODE_S]) { --dest.y; fID=idle_d; }
    else if (key_states[SDL_SCANCODE_D]) { ++dest.x; fID=idle_r; }
    else fID= fsm.get_fid();
 
    if (dest.x == x_position && dest.y == y_position) // if no movement
      return;                                            // no need to do anything further

    fsm.input(fID); // may need a "turn_around" animation that takes a couple frames to delay input.

    const Warp::Warp_Point * warps = Warp::get_warp_table(mID);
    for (int i = 0; i < Warp::num_warps[mID]; ++i)         // for each warp point
    {
      if (dest.x == warps[i].p.x && dest.y == warps[i].p.y) { // if the destination is equal to the current warp point
        Debug::log_from(Debug::player,"warp point found");       // log a message that warp point has been identified
        if ( !suspend_warp(warps[i].dst) ) {                        // should the player take one final step before the warp takes place?
          do_warp(warps[i].dst);
        }
        return;
      }
    }

    x_max = maps[0]->w_tiles;
    y_max = maps[0]->h_tiles;

    Point spawn = dest; // this is of use when the player is trying to move to a neighbor map
    if (wnode->in_overworld) { // in overworld logic: prepare to potentially check neighbor map
      if (dest.y == y_max) {
        mID = wnode->up();           // change the mID to determine which p_collision to retrieve
        dest.y = 0;                  // "overflow" destination coordinate
        spawn.y = -1;                // update the location the player will need to be placed for the impending map update
        y_max = maps[1]->h_tiles;    // set new boundaries so that collision detection uses proper bounds
        x_max = maps[1]->w_tiles;
      }
      else if (dest.x == -1) {
        mID = wnode->left();
        dest.x = maps[2]->w_tiles - 1;
        spawn.x = maps[2]->w_tiles;
        y_max = maps[2]->h_tiles;
        x_max = maps[2]->w_tiles;
      }
      else if (dest.y == -1) {
        mID = wnode->down();
        dest.y = maps[3]->h_tiles - 1;
        spawn.y = maps[3]->h_tiles;
        y_max = maps[3]->h_tiles;
        x_max = maps[3]->w_tiles;
      }
      else if (dest.x == x_max) {
        mID = wnode->right();
        dest.x = 0;
        spawn.x = -1;
        y_max = maps[4]->h_tiles;
        x_max = maps[4]->w_tiles;
      }
    }
    p_collision = Collision_Data::get_ptr(mID);
    Debug::log_from(Debug::player,"collision ptr for mID: ",+mID);
    bool can_move = can_move_to(dest);
    if ( can_move ) {
      if (fsm == fID && frame_prevent_interupt_counter == 0) {
        this->start_animation();
        if (mID != wnode->mID) {
          scene_manager.update_map(mID);
          this->set_position(spawn.x, spawn.y);
        }
      }
    }
    else {
      // TODO: add a frame deley counter solution for this so that it does not spam when walking into something.
      scene_manager.sound.play_sfx(bump_wall);
    }
  }
}

// Sends updated matrix data to the PlayerSprite shader and issues an OpenGL draw call.
void Player::render()
{
  glUseProgram(shader.handle);
  shader.set("MVP", cam.get_ViewToProjection_Matrix() * mv);
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, n_verts, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}

// Shift the position of the player as well as the camera
void Player::set_position(int _x, int _y)
{
  x_position= _x;
  y_position= _y;
  float x = (float)_x + 0.25f;
  float y = (float)_y + 0.5f;
  vec3 xyCoordinates= vec3(x, y, .001f);
  model= translate(mat4(1.0f), xyCoordinates);
  cam.set_Position(vec3(x, y, 5.0f));
}

void Player::set_position(const Warp::Destination& dest)
{
  this->set_position(dest.spawn_point.x, dest.spawn_point.y);
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
  Debug::log_from(Debug::player,"performing warp");
  fsm.input(dest.fID);                // input the facing direction
  scene_manager.update_map(dest.mID); // sets current world node and changes maps array
  this->set_position(dest);           // update the player position
  if (dest.step_after)
    start_animation();
}

// Use this function to process any extra key press logic for player
void Player::take_input(const Uint8* key_states)
{
  if (key_states[SDL_SCANCODE_P]) std::cout << "[Player] position = " << x_position << ',' << y_position << '\n';
  if (key_states[SDL_SCANCODE_F]) std::cout << "[Player] current frame state: " << fsm.get_fid() << '\n';
  if (key_states[SDL_SCANCODE_0]) maps[0]->is_visible = !maps[0]->is_visible;
}

void Player::update_frame_to_stride()
{
  if      (fsm == idle_l) fsm.input(stride_l1);
  else if (fsm == idle_r) fsm.input(stride_r1);
  else if (fsm == idle_d) fsm.input((stride_left = !stride_left) ? stride_d1 : stride_d2);
  else if (fsm == idle_u) fsm.input((stride_left = !stride_left) ? stride_u1 : stride_u2);
  else Debug::log_error("[Player] failed to update frame to stride");
}
    
void Player::update_frame_to_idle()
{
  if      (fsm == stride_l1)                     fsm.input(idle_l);
  else if (fsm == stride_r1)                     fsm.input(idle_r);
  else if (fsm == stride_d1 || fsm == stride_d2) fsm.input(idle_d);
  else if (fsm == stride_u1 || fsm == stride_u2) fsm.input(idle_u);
  else Debug::log_error("[Player] failed to update frame to idle");
}

void Player::update_position()
{
  auto change_position = [&](float dx, float dy) {
    vec3 x_y_translation= vec3(dx, dy, 0.0f);  // Create a vector with the given offsets
    model = translate(model, x_y_translation); // Translate the model matrix of the player with the vector above
    cam.move(x_y_translation);                 // Apply that same translation to the games camera
  };
  float delta_pos= 0.0625f; // 1/16th because 16 frames total in the walk animation --> the player will move exactly 1 unit total per step taken
  if (fsm == idle_l || fsm == stride_l1) change_position(-delta_pos, 0.0f);
  else if (fsm == idle_r || fsm == stride_r1) change_position(delta_pos, 0.0f);
  else if (fsm == idle_u || fsm == stride_u1 || fsm == stride_u2) change_position(0.0f, delta_pos);
  else if (fsm == idle_d || fsm == stride_d1 || fsm == stride_d2) change_position(0.0f, -delta_pos);
  else Debug::log_error("[Player] failed to update position");
}

void Player::walk_anim_end() 
{
  // update position
  if      (fsm == idle_u) ++y_position;
  else if (fsm == idle_l) --x_position;
  else if (fsm == idle_d) --y_position;
  else if (fsm == idle_r) ++x_position;
  else Debug::log_error("[Player] error: failed to update coordinates, animation state may be incorrect");
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
  if     (*frame_counter < 1)   Debug::log_error_abort("[Player] error: walk_animation frame_counter should be at least 1 and was not");
  if     (*frame_counter == 4)  update_frame_to_idle();
  else if(*frame_counter == 12) update_frame_to_stride();
  update_position();
  (*frame_counter)--;
  if (*frame_counter == 0) walk_anim_end();
}