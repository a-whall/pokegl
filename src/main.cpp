#include "game.h"
#undef main // something to do with SDL having already defined main
int main(int argc, char* argv[])
{
  constexpr int window_width = 960, window_height = 640; // in pixels.   3:2 aspect ratio
  constexpr int fps = 60, ms_per_frame = static_cast<int>(1000/fps + 0.5f);
  Uint32 frame_begin, dt;
  Game::Instance *app = new Game::Instance();
  app->init(argv[0], SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  while( app->is_running() )
  {
    frame_begin = SDL_GetTicks();
    app->step(ms_per_frame);
    dt = SDL_GetTicks() - frame_begin;
    //std::cout << dt << "\n";
    if (dt < ms_per_frame) SDL_Delay(ms_per_frame - dt);
    else std::cout << "fdt > fps\n";
  }
  app->clean();
}