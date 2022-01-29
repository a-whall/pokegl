#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include "camera.h"
#include "scene.h"

struct App_Stats
{
  float cma_fdt= 0.0f; // cumulative-moving-average of frame-delta-time in milliseconds
};

struct SDL_App_Data
{
  SDL_Window *p_window;
  SDL_GLContext context;
  const Uint8 *p_key_states;
  Mix_Music *p_music;
};

class Application
{
protected:
  App_Stats app_stats;
  SDL_App_Data sdl;
  Scene::Manager scene_manager;
  bool running=false;
  void print_opengl_extensions();
  void print_nvidia_memory_info();
public:  
  void init(const char*, int, int, int, int, int);
  void step(float);
  void update_fdt_average(Uint32);
  void clean();
  bool is_running();
  virtual void on_init();
  virtual void on_update(float);
  virtual void on_exit();
private:
  void prep_scene();
  void init_library_SDL();
  void init_window_and_keystates_SDL(const char*, int, int, int, int, int);
  void config_opengl_context_SDL();
  void wrangle_modern_opengl_api_GLEW();
  void config_opengl_debug_flags();
};