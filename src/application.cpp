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
  this->on_init();
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
    this->on_update(frame_time);      // overridable for class user to implement
  }
  {// render
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // clear frame-buffer data
    scene_manager.render();                           // let all scene objects render to frame-buffer
    SDL_GL_SwapWindow(sdl.p_window);                  // present frame-buffer to window
  }
}

void Application::update_fdt_average(Uint32 dt)
{
  // This function will last for 2^32 frames before cnt overflows
  // 2^32 / 60 = number of seconds this can run at 60 fps = 71,000,000s
  // = 1,193,000 minutes = 20,000 hours = 828 days. so no worries.
  static unsigned cnt= 0;
  stats.cma_fdt += (dt-stats.cma_fdt)/(++cnt);
}

void Application::clean()
{
  this->on_exit();                   // call user-overridden exit function
  scene_manager.clean();             // delete scene objects
  Debug::log_from(Debug::stats,"average frame time: ",stats.cma_fdt," ms"); // may change this to print other program stats as well 
  Mix_CloseAudio();                  // shut down and cleanup SDL_Mixer API
  SDL_GL_DeleteContext(sdl.context); // destroy OpenGL instance
  SDL_DestroyWindow(sdl.p_window);   // close app window
  IMG_Quit();                        // unload any dynamically allocated image library handles (can be called after last call to IMG_Load but I call it here)
  SDL_Quit();                        // shut down initialized subsystems
}

bool Application::is_running()
{
  return running;
}

/////////////////////////////////////////////////
// These functions are meant to be overridden  //
/////////////////////////////////////////////////
void Application::on_init() {}                 //
void Application::on_update(float t) {}        //
void Application::on_exit() {}                 //
/////////////////////////////////////////////////

// enables some OpenGL rendering capabilities
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
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)         // initialize SDL, as well as flagged, subsystems
    Debug::log_error_abort(Debug::application,SDL_GetError());
  if (IMG_Init(IMG_INIT_PNG) == 0)                           // load image-format support indicated by flag(s)
    Debug::log_error_from(Debug::application,IMG_GetError());
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) // initialize SDL_Mixer API
    Debug::log_error_from(Debug::application,Mix_GetError());
  if (!Mix_Init(MIX_INIT_MP3))                               // init file support for MP3. Note: no need to call Mix_Init for wav (simply returns true if already initialized)
    Debug::log_error_from(Debug::application,Mix_GetError());
  Debug::log_from(Debug::application,"SDL subsystems initialized");
}

// Create a window with the exact same paremeters of Application::init. Initialize a pointer to SDL's keystates array which lets you know which keys are pressed (the array is updated by calling 'SDL_PumpEvents')
void Application::init_window_and_keystates_SDL(const char* title, int x, int y, int w, int h, int fullscreen)
{
  sdl.p_window= SDL_CreateWindow(title, x, y, w, h, fullscreen);
  scene_manager.key_states= sdl.p_key_states= SDL_GetKeyboardState(nullptr);
  Debug::log_from(Debug::application,"SDL window initialized");
  scene_manager.init_camera(x,y);
  Debug::log_from(Debug::application,"camera initialized");
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
  Debug::log_from(Debug::application,"OpenGL context attributes configured via SDL");
}

void Application::wrangle_modern_opengl_api_GLEW()
{
  glewExperimental = GL_TRUE; // declared in glew.h
  GLenum glew_error = glewInit();
  if (glew_error != GLEW_OK)
    Debug::log_error_from(Debug::application,"GLEW failed to initialize: ",glewGetErrorString(glew_error));
  Debug::log_from(Debug::application,"OpenGL function pointers loaded");
}

// enables the output of debug messages produced by a debug context. If it instead were left disabled, the debug message log would be silenced. Note that in a non-debug context, very few, if any messages might be produced, even when GL_DEBUG_OUTPUT is enabled. 
void Application::config_opengl_debug_flags()
{
  glEnable(GL_DEBUG_OUTPUT);
  Debug::submit_debug_callback();
  Debug::log_from(Debug::application,"OpenGL debug output enabled");
}

// Print the OpenGL extensions implemented by this hardware as a list. This function is here if you want to check for yourself what your hardware implements at first glance. This function is protected so the best place to call it would be to derive from Application and call this function from the on_init() override.
void Application::print_opengl_extensions()
{
  GLint n_extensions, i;
  glGetIntegerv(GL_NUM_EXTENSIONS, &n_extensions);
  Debug::log("[Application] ",n_extensions," OpenGL extensions found to be implemented by this hardware");
  for (i = 0; i < n_extensions; i++)
  {
    Debug::log("\t\t",glGetStringi(GL_EXTENSIONS,i));
  }
}