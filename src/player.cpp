#include "player.h"
#include "SDL_image.h" // IMG_Load
#include "gtc/matrix_transform.hpp"
#include "camera.h"
#include "frameid.h"

// Player constructor : Initialize a bunch of player state values. Starts the player facing down.
// @param cam: a reference to the scene camera
// @param active_map: a pointer to a pointer which points to the active map (the central map tile of the scene)
// @param s: a reference to shader for sprite animation
Player::Player(Camera &cam, Map** active_map, Shader &s)
: Sprite(0.25f, 0.5f, .001f, cam, s),
  player_animation_frame_state()
{
  frame_prevent_interupt_counter = stride_left = x_position = y_position = 0;
  player_animation_frame_state.set_shader(&shader);                                 
  init_buffers();
  init_animations();
  glUseProgram(shader.handle);
  shader.set("frameID", e_down_idle);
  glUseProgram(0);
  pp_active_map= active_map;
}

// Player destructor : Delete all OpenGL objects associated with this player
Player::~Player()
{
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glDeleteVertexArrays(1, &vao);
  glDeleteTextures(1, &opengl_texture_ID);
}

// Frames are loaded using IMG_Load() from the SDL_image library. This function creates an OpenGL texture2D_array texture object which is intended to be cleaned up by the player destructor.
// @warning IMG_INIT_PNG flag must be passed to IMG_INIT, handled by application
void Player::init_animations()
{
  constexpr int frame_width_pixels= 16,
                frame_height_pixels= frame_width_pixels,
                layer_count= 10;
  constexpr size_t frame_size_bytes= frame_width_pixels * frame_height_pixels * 4; // 8 bits-per-color-channel & RGBA => 4 bytes per pixel
  SDL_Surface *loaded_image= nullptr;
  GLubyte *p_pixel_data= new GLubyte[frame_size_bytes * layer_count];
  unsigned offset= 0;

  std::vector<const char*> frames =
  { // note: order matches Frame_ID enum
    "assets/sprite/player/forward_idle.png",
    "assets/sprite/player/backward_idle.png",
    "assets/sprite/player/left_idle.png",
    "assets/sprite/player/right_idle.png",
    "assets/sprite/player/forward_stride1.png",
    "assets/sprite/player/forward_stride2.png",
    "assets/sprite/player/backward_stride1.png",
    "assets/sprite/player/backward_stride2.png",
    "assets/sprite/player/left_stride.png",
    "assets/sprite/player/right_stride.png"
  };

  glActiveTexture(GL_TEXTURE0); // specify which GL_TEXTURE unit subsequent texture state calls will affect
  glGenTextures(1, &opengl_texture_ID);
  glBindTexture(GL_TEXTURE_2D_ARRAY, opengl_texture_ID);

  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, frame_width_pixels, frame_height_pixels, layer_count); // allocate GPU memory
  for (const char *file_path : frames)
  { // Load each frame's pixel data from file into a GLubyte array
    loaded_image = IMG_Load(file_path);
    if (loaded_image == nullptr)
      Debug::log_error_abort("[Player] Player::init_animations failed to load a texture:", file_path);
    memcpy(p_pixel_data + offset, loaded_image->pixels, frame_size_bytes);
    SDL_FreeSurface(loaded_image);
    offset += frame_size_bytes;
  }
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, frame_width_pixels, frame_height_pixels, layer_count, GL_RGBA, GL_UNSIGNED_BYTE, p_pixel_data); // send pixel data to GPU memory
  delete p_pixel_data; // free pixel data from heap

  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  player_animation_frame_state
    .create_state( "D_IDLE",    e_down_idle,    {{"l","L_IDLE"},{"r","R_IDLE"},{"u","U_IDLE"},{"d1","D_STRIDE1"},{"d2","D_STRIDE2"}})
    ->create_state("U_IDLE",    e_up_idle,      {{"l","L_IDLE"},{"r","R_IDLE"},{"d","D_IDLE"},{"u1","U_STRIDE1"},{"u2","U_STRIDE2"}})
    ->create_state("L_IDLE",    e_left_idle,    {{"r","R_IDLE"},{"u","U_IDLE"},{"d","D_IDLE"},{"ls","L_STRIDE" }})
    ->create_state("R_IDLE",    e_right_idle,   {{"l","L_IDLE"},{"u","U_IDLE"},{"d","D_IDLE"},{"rs","R_STRIDE" }})
    ->create_state("L_STRIDE",  e_left_stride,  {{"l","L_IDLE"}})
    ->create_state("R_STRIDE",  e_right_stride, {{"r","R_IDLE"}})
    ->create_state("D_STRIDE1", e_down_stride1, {{"d","D_IDLE"}})
    ->create_state("D_STRIDE2", e_down_stride2, {{"d","D_IDLE"}})
    ->create_state("U_STRIDE1", e_up_stride1,   {{"u","U_IDLE"}})
    ->create_state("U_STRIDE2", e_up_stride2,   {{"u","U_IDLE"}});
}

// VBO and EBO data is created to exist temporarily on stack and, from this function, is sent to the GPU. VBO data represents model coordinates of the player texture, i.e. a 1x1 quad centered on origin, as well as texture coordinates for texture mapping which is handled by OpenGL. EBO data represents the order in which vertices in the VBO are to be drawn to make triangles by the OpenGL draw calls.
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

 // Updates the players model-view matrix using the camera's view matrix. Then checks whether to accept input or continue executing an animation phase.
 // @param t: the intended duration of frame of the application
 // @param key_states: a pointer to SDL pressed keys data
void Player::update(float t, const Uint8* key_states)
{
  mv= cam.get_WorldToView_Matrix() * model;
  if (frame_prevent_interupt_counter)
    walk_animation(&frame_prevent_interupt_counter);
  else
  {
    take_input(key_states);
    // // take_input(key_states);
    // Point input_direction=
    //   key_states[SDL_SCANCODE_W] ? Point{ 0, 1} :
    //   key_states[SDL_SCANCODE_A] ? Point{-1, 0} :
    //   key_states[SDL_SCANCODE_S] ? Point{ 0,-1} :
    //   key_states[SDL_SCANCODE_D] ? Point{ 1, 0} :
    //   Point{0,0};
    
    // Point destination= Point{
    //   x_position + input_direction.x,
    //   y_position + input_direction.y
    // };

    // // if destination point is a warp point

    // if ((*pp_active_map)->allows_move_to(destination.x, destination.y)) {
    //   std::cout << "map allows move to : " << destination.x << ',' << destination.y << '\n';
    //   return;
    // }
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

// Process key input based on keys pressed: move keys {w,a,s,d}
void Player::take_input(const Uint8* key_states)
{

  //Point move_vector = get_input_direction();
  //Point dest_point = { move_vector.x + x_position, move_vector.y + y_position };

  //if ((*pp_active_map)->allows_move_to(dest_point.x, dest_point.y))
  // Might be possible to have a check in here that finds that the requested position may be off map.
  if (key_states[SDL_SCANCODE_W])
  {
    // if the map allows a move up
    if ((*pp_active_map)->allows_move_to(x_position, y_position + 1))
      // if already facing up and there is no currently executing animation
      if (player_animation_frame_state == "U_IDLE" && frame_prevent_interupt_counter == 0)
        // prepare to start a new animation which takes 16 frames
        frame_prevent_interupt_counter= 16;

    // switch facing direction
    player_animation_frame_state.input_event("u");
  } 
  else if (key_states[SDL_SCANCODE_A])
  {
    if ((*pp_active_map)->allows_move_to(x_position - 1, y_position))
      if (player_animation_frame_state == "L_IDLE" && frame_prevent_interupt_counter == 0)
        frame_prevent_interupt_counter= 16;
    player_animation_frame_state.input_event("l");
  }
  else if (key_states[SDL_SCANCODE_S])
  {
    if ((*pp_active_map)->allows_move_to(x_position, y_position - 1))
      if (player_animation_frame_state == "D_IDLE" && frame_prevent_interupt_counter == 0)
        frame_prevent_interupt_counter= 16;
    player_animation_frame_state.input_event("d");
  }
  else if (key_states[SDL_SCANCODE_D])
  {
    if ((*pp_active_map)->allows_move_to(x_position + 1, y_position)) 
      if (player_animation_frame_state == "R_IDLE" && frame_prevent_interupt_counter == 0)
        frame_prevent_interupt_counter= 16;
    player_animation_frame_state.input_event("r");
  }

  if (key_states[SDL_SCANCODE_P])
  {
    std::cout << "[Player] position = " << x_position << ',' << y_position << '\n';
  }
}

void Player::update_frame_to_stride()
{
  if (player_animation_frame_state == "L_IDLE")
    player_animation_frame_state.input_event("ls");
  else if (player_animation_frame_state == "R_IDLE")
    player_animation_frame_state.input_event("rs");
  else if (player_animation_frame_state == "D_IDLE")
    player_animation_frame_state.input_event((stride_left = !stride_left) ? "d1" : "d2");
  else if (player_animation_frame_state == "U_IDLE")
    player_animation_frame_state.input_event((stride_left = !stride_left) ? "u1" : "u2");
}
    
void Player::update_frame_to_idle()
{
  if (player_animation_frame_state == "L_STRIDE")
    player_animation_frame_state.input_event("l");
  else if (player_animation_frame_state == "R_STRIDE")
    player_animation_frame_state.input_event("r");
  else if (player_animation_frame_state == "D_STRIDE1" || player_animation_frame_state == "D_STRIDE2")
    player_animation_frame_state.input_event("d");
  else if (player_animation_frame_state == "U_STRIDE1" || player_animation_frame_state == "U_STRIDE2")
    player_animation_frame_state.input_event("u");
}

// Create a vector with the given offsets
// Translate the model matrix of the player with the vector above
// Apply that same translation to the games camera
void Player::change_position(float dx, float dy)
{
  vec3 x_y_translation= vec3(dx, dy, 0.0f);
  model = translate(model, x_y_translation);
  cam.move(x_y_translation);
}

void Player::update_position()
{
  float delta_pos= 0.0625f;
  if (player_animation_frame_state == "L_IDLE" || player_animation_frame_state == "L_STRIDE")
    change_position(-delta_pos, 0.0f);
  else if (player_animation_frame_state == "R_IDLE" || player_animation_frame_state == "R_STRIDE")
    change_position(delta_pos, 0.0f);
  else if (player_animation_frame_state == "U_IDLE" || player_animation_frame_state == "U_STRIDE1" || player_animation_frame_state == "U_STRIDE2")
    change_position(0.0f, delta_pos);
  else if (player_animation_frame_state == "D_IDLE" || player_animation_frame_state == "D_STRIDE1" || player_animation_frame_state == "D_STRIDE2")
    change_position(0.0f, -delta_pos);
}

void Player::walk_anim_end_update_coordinates() {
  if (player_animation_frame_state == "L_IDLE")
    --x_position;
  else if (player_animation_frame_state == "R_IDLE")
    ++x_position;
  else if (player_animation_frame_state == "D_IDLE")
    --y_position;
  else if (player_animation_frame_state == "U_IDLE")
    ++y_position;
}

// Updates frames at specific frame counts over a 16 frame interval.
// Frame counter decrements invariably, therefore this function shouldn't be called unless *frame_counter is greater than zero.
// 4 frames are spent in idle pose, 8 frames in stride pose, then 4 frames are spent back in idle
// so if you keep walking there is an equal frame interval.
// Assumes 60 FPS.
void Player::walk_animation(int *frame_counter)
{
  if (*frame_counter < 1)
    Debug::log_error_abort("[Player] error: walk_animation frame_counter should be at least 1 and was not.");

  if (*frame_counter == 4)
    update_frame_to_idle();

  else if(*frame_counter == 12)
    update_frame_to_stride();

  update_position();

  (*frame_counter)--;

  if (*frame_counter == 0)
    walk_anim_end_update_coordinates();
}