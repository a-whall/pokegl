#pragma once
#include "SDL.h"
#include "GL/glew.h"
#include "sprite.h"
#include "player.h"
#include "map.h"
#include "camera.h"
#include "shader.h"

namespace Scene
{
  using std::vector;
  
  class Manager
  {
    vector<Player*> sprites;
    vector<Shader*> shaders;
    Map * maps[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
  public:
    World_Graph * world_graph= nullptr;
    Camera * camera_controller= nullptr;
    const Uint8 * key_states= nullptr;
    void init_camera(int, int);
    Player& new_player(Shader& s);
    Shader& new_shader(const char *);
	  Map&    init_map(Shader& s);
    void update(float t);
    void render();
    void clean();
    void refresh();
  };
}