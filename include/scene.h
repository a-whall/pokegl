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
#include "text.h"



namespace Scene
{
  using std::unordered_map;
  using std::vector;



  class Manager
  {
    vector<Player*> sprites;
    vector<Shader*> shaders;

  public:

    GLuint va, eb, cvb, blvb; // sprite vertex array,  quad element buffer,  center-aligned quad vertex buffer,  bottom-left-aligned quad vertex buffer. (alignment with origin)
    Map_Manager * map_manager;
    Text_Manager * text_manager; // manages its own va
    Sound_Manager sound;
    World_Graph * world_graph= nullptr;
    Camera * camera_controller= nullptr;
    const Uint8 * key_states= nullptr;

    Player & new_player(Shader & s);
    Shader & new_shader(const char *);

    void init_camera(int w, int h);
    void init_maps();
    void init_text();
    void init_buffers();

    void sync_world(Map_ID);

    void update(float t);
    void render();

    void clean();
    void refresh();
  };
}