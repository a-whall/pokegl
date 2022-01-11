#include "game.h"
#undef main // fixes bug that prevents the program from running on windows. something to do with SDL


int main(int argc, char* argv[])
{
  constexpr int window_width = 960, window_height = 640; // in pixels. 3:2 aspect ratio
  constexpr int fps(60), ms_per_frame(1000/fps + 0.5f);

  PokeGL app;
  app.init(argv[0], SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_OPENGL);
  while( app.is_running() )
  {
    app.step(ms_per_frame); // The value passed to step is constant because some graphics operations depend on it (e.g. speed at which a sprite moves per frame)
  }
  app.clean();
}