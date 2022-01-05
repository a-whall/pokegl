#pragma once
#include "SDL.h"
#include "GL/glew.h"
#include "sprite.h"
#include "player.h"
#include "map.h"
#include "camera.h"
#include "shader.h"
#include "sound.h"
#include "map_orientation.h"

namespace Scene
{
  using std::unordered_map;
  using std::vector;
  
  class Manager
  {
    vector<Player*> sprites;
    vector<Shader*> shaders;
    //Map * maps[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
  public:
    unordered_map<Map_Orientation,Map*> maps;
    Sound_Manager sound;
    World_Graph * world_graph= nullptr;
    Camera * camera_controller= nullptr;
    const Uint8 * key_states= nullptr;
    void init_camera(int, int);
    Player& new_player(Shader& s);
    Shader& new_shader(const char *);
    void    init_maps(Shader& s);
    void update_map(Map_ID_enum);
    void update(float t);
    void render();
    void clean();
    void refresh();
  };
}