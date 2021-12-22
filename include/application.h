#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include "camera.h"
#include "scene.h"


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
	SDL_App_Data sdl;
  Scene::Manager scene_manager;
	bool running= false;

public:
  
	void init(const char*, int, int, int, int, int);
  void step(float);
  void clean();
	bool is_running();
  virtual void on_init();
  virtual void on_update(float);

private:

	void prep_scene();
	void init_library_SDL();
	void init_window_and_keystates_SDL(const char*, int, int, int, int, int);
	void config_opengl_context_SDL();
	void wrangle_modern_opengl_api_GLEW();
	void config_opengl_debug_flags();

protected:

	void print_opengl_extensions();
	void set_music(const char* MP3_file);
};