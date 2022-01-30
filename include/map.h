#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <unordered_map>
#include <vector>
#include "mapid.h"
#include "map_orientation.h"
#include "texid.h"



namespace Scene { class Manager; };
class Camera;
class Shader;
class World_Node;



const char * get_tex_path(Tex_ID);
const char * get_tex_path(Map_ID);



class Map_Manager
{
  Scene::Manager & scene_manager;
  GLuint ivb, t[max_tex_id]; // instanced buffer, texture array which will store the gl texture object id for all map textures
  Shader & map_shader;
  std::vector<float> ibuf; // 0 initialized
  int counter;
  int n_active_maps= 1;

public:

  Map_Manager(Scene::Manager &man);
  ~Map_Manager();

  void update(float, const Uint8 *);
  void render();
  void try_to_load_texture(Map_ID, int);
  void update_maps(World_Node *);
};