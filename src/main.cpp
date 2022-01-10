#include "game.h"
#undef main // fixes bug that prevents the program from running on windows. something to do with SDL having already defined main
int main(int argc, char* argv[])
{
  using namespace Debug;
  constexpr int window_width = 960, window_height = 640; // in pixels.   3:2 aspect ratio
  constexpr int fps = 60, ms_per_frame = static_cast<int>(1000/fps + 0.5f);
  Uint32 frame_begin, dt;
  Game::Instance app;
  app.init(argv[0], SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  while( app.is_running() )
  {
    frame_begin= SDL_GetTicks();
    app.step(ms_per_frame); // The value passed to step must be constant because some graphic operations depend on it (e.g. speed at which a sprite moves per frame)
    app.update_fdt_average(dt = SDL_GetTicks() - frame_begin);
    if (dt < ms_per_frame)
      SDL_Delay(ms_per_frame - dt);
  }
  app.clean();
  log<std::cout>("[ ",magenta,"Exit",reset," ] ",magenta,EXIT_SUCCESS,reset);
}