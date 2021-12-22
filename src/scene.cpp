#include <algorithm>
#include "scene.h"
#include "gtc/matrix_transform.hpp"

namespace Scene
{
  void Manager::init_camera(int x, int y)
  {
    float aspect_ratio = x / (float)y;
    this->camera_controller = new Camera(glm::vec3(0, 0, 5), glm::vec3(0, 0, -1), aspect_ratio);
  }

  Map& Manager::new_map(Map_ID_enum mID, Camera& c, Shader& s)
  {
    maps.push_back(new Map(mID, c, s));
    return *maps.back();
  }

  Shader& Manager::new_shader(const char* file_name)
  {
    shaders.push_back(new Shader(glCreateProgram()));
    compile(shaders.back()->handle, file_name);
    log(shaders.back()->handle, GL_PROGRAM_INPUT);
    log(shaders.back()->handle, GL_UNIFORM);
    return *shaders.back();
  }

  void Manager::update(float t)
  {
    for (auto& m : maps)    m->update(t, keystates);
	  for (auto& s : sprites) s->update(t, keystates);
	}

  void Manager::render()
  {
    for (auto& m : maps)    m->render();
	  for (auto& s : sprites) s->render(); 
	}

  void Manager::clean()
  {
    for (auto& s: sprites) delete s;
    for (auto& m: maps)    delete m;
    for (auto& s: shaders) delete s;
  }

  void Manager::refresh()
  {
    using std::remove_if;
    using std::begin;
    using std::end;

    auto is_active = [](const Sprite* sprite) { return !sprite->is_active; };
		sprites.erase( remove_if(begin(sprites),end(sprites),is_active), end(sprites) );
    maps.erase( remove_if(begin(maps),end(maps),is_active), end(maps));
	}
}