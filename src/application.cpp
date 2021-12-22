#include "application.h"
#include <SDL_image.h>
#include "shader.h"
#include "collision.h"
#include "map.h"
#include "animation.h"
#include "player.h"
#include "debug.h"

// Startup function : prepares the window and OpenGL context using SDL, exposes OpenGL function pointers with GLEW, initializes the scene camera
void Application::init(const char* title, int x, int y, int w, int h, int fullscreen)
{
  // each function called below which contains a library call has _libName appended
  init_library_SDL();
	init_window_and_keystates_SDL(title, x, y, w, h, fullscreen);
	config_opengl_context_SDL();
	wrangle_modern_opengl_api_GLEW();
	config_opengl_debug_flags();
	prep_scene();
  on_init();
  this->running= true;
}

void Application::step(float frame_time)
{
  {// handle IO events
    SDL_Event event;
    while (SDL_PollEvent(&event)) // poll for pending events
    {
      // clicking the window's red X generates this event
      if (event.type == SDL_QUIT)
        running= false;
      // press escape to exit
	    if (event.type == SDL_KEYUP
      && event.key.keysym.sym == SDLK_ESCAPE)
        running= false;
      // scroll in the z axis for 2D zoom
      if (event.type == SDL_MOUSEWHEEL)
        scene_manager.camera_controller->move(glm::vec3(0, 0, -event.wheel.y));
    }
  }
  {// update
    scene_manager.refresh();          // remove any scene objects that have had their 'inactive' flag set
	  scene_manager.update(frame_time); // update all scene objects
    on_update(frame_time);            // overridable for class user to implement
  }
  {// render
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // clear frame-buffer data
	  scene_manager.render();                           // let all scene objects render to frame-buffer
	  SDL_GL_SwapWindow(sdl.p_window);                  // present frame-buffer to window
  }
}

void Application::clean()
{
  scene_manager.clean();           // delete scene objects
  if (sdl.p_music != nullptr)        // if music is currently loaded
		Mix_FreeMusic(sdl.p_music);         // turn it off
  Mix_CloseAudio();                // shut down and cleanup SDL_Mixer API
	SDL_GL_DeleteContext(sdl.context); // destroy OpenGL instance
	SDL_DestroyWindow(sdl.p_window);      // close app window
	IMG_Quit();                      // unload any dynamically allocated image library handles (can be called after last call to IMG_Load but I call it here)
	SDL_Quit();                      // shut down initialized subsystems
}

bool Application::is_running()
{
  return running;
}

//////////////////////////////////////////
// These 2 functions are to be override //
//////////////////////////////////////////
void Application::on_init() {}          //
void Application::on_update(float t) {} //
//////////////////////////////////////////

// Sets some OpenGL functionality
void Application::prep_scene()
{
	glClearColor(0.02f, 0.02f, 0.02f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Initialize SDL, SDL_image, and SDL_mixer libraries as well as support for PNG image format and for MP3 file format. A failure to initialize the main SDL library will cause the program to abort but all other errors are simply reported to cerr.
// Note: SDL_init() automatically initializes the Event Handling, File I/O and Threading subsystems.
void Application::init_library_SDL()
{
  using namespace Debug;
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)         // initialize SDL, as well as flagged, subsystems
    log_error_abort("[Application] ", SDL_GetError());
	if (IMG_Init(IMG_INIT_PNG) == 0)                           // load image-format support indicated by flag(s)
    log_error("[Application] ", IMG_GetError());
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) // initialize SDL_Mixer API
    log_error("[Application] ", Mix_GetError());
  if (!Mix_Init(MIX_INIT_MP3))                               // initialize audio support for MP3 files (does nothing but return true if already initialized)
    log_error("[Application] ", Mix_GetError());
  log("[Application] ","SDL subsystems initialized");
}

// Create a window with the exact same paremeters of Application::init. Initialize a pointer to SDL's keystates array which lets you know which keys are pressed (the array is updated by calling 'SDL_PumpEvents')
void Application::init_window_and_keystates_SDL(const char* title, int x, int y, int w, int h, int fullscreen)
{
	sdl.p_window= SDL_CreateWindow(title, x, y, w, h, fullscreen);
	sdl.p_key_states= SDL_GetKeyboardState(nullptr);
  scene_manager.keystates= sdl.p_key_states;
  Debug::log("[Application] ","SDL window initialized");
  scene_manager.init_camera(x,y);
  Debug::log("[Application] ","camera initialized");
}

// Create an OpenGL context, glsl = version 4.5, core profile => forward compatibility glsl
void Application::config_opengl_context_SDL()
{
  sdl.context = SDL_GL_CreateContext(sdl.p_window);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
  Debug::log("[Application] ","OpenGL context attributes configured via SDL");
}

void Application::wrangle_modern_opengl_api_GLEW()
{
  glewExperimental = GL_TRUE;
	GLenum glewERR = glewInit();
	if (glewERR != GLEW_OK)
		Debug::log_error("[Application] ","glew error : ", glewGetErrorString(glewERR));
  Debug::log("[Application] ","OpenGL function pointers loaded");
}

void Application::config_opengl_debug_flags()
{
  glEnable(GL_DEBUG_OUTPUT);
	Debug::submitDebugCallbackFunction();
}

// Get the number of extensions implemented by hardware, print them as a list
void Application::print_opengl_extensions()
{
	GLint n_extensions;
  glGetIntegerv(GL_NUM_EXTENSIONS, &n_extensions);
  std::cout << "[Application] " << n_extensions << " OpenGL extensions found to be implemented by this hardware\n";
	for (int i = 0; i < n_extensions; i++)
		std::cout << "\t\t" << glGetStringi(GL_EXTENSIONS, i) << "\n";
}

// load an MP3 file, and start playing it. Initializes support for MP3 if it wasnt already initialized.
void Application::set_music(const char* MP3_file)
{
  if (sdl.p_music != nullptr)
  { // if music is currently loaded                     
    Mix_FreeMusic(sdl.p_music); // turn it off
    sdl.p_music = nullptr;      // nullify its pointer
  }
  // load an MP3 file and set music pointer
	if (sdl.p_music = Mix_LoadMUS(MP3_file))
    Mix_PlayMusic(sdl.p_music, -1); // start music
	else
    Debug::log_error("[Application] ", Mix_GetError());
}