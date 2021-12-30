#include <algorithm>
#include "scene.h"
#include "gtc/matrix_transform.hpp"
#include "world.h"

namespace Scene
{
  void Manager::init_camera(int x, int y)
  {
    float aspect_ratio = x / (float)y;
    this->camera_controller = new Camera(glm::vec3(0, 0, 5), glm::vec3(0, 0, -1), aspect_ratio);
  }

  Player& Manager::new_player(Shader& s)
  {
    Camera& c = *this->camera_controller;
    sprites.push_back(new Player(c, this->world_graph, s));
    sprites.back()->maps = this->maps;
    return *sprites.back();
  }

  Map& Manager::init_map(Shader& s)
  {
    World_Node * active_wnode = world_graph->get_current_node();
    if (active_wnode == nullptr)
      Debug::log_error_abort("[Scene] manager expected the world_graph to have a current world node");
    Camera& c = *this->camera_controller;
    maps[0] = new Map(active_wnode->mID, c, s);
    maps[1] = new Map(null_map_id, c, s); // up
    maps[2] = new Map(null_map_id, c, s); // left
    maps[3] = new Map(null_map_id, c, s); // down
    maps[4] = new Map(null_map_id, c, s); // right
    


    return *maps[0];
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
    for (auto& m : maps)    m->update(t, key_states);
	  for (auto& s : sprites) s->update(t, key_states);
	}

  void Manager::render()
  {
    // if the active world node is flagged as being 'in_overworld' then render all maps
    // otherwise print only the main map (which would be a self contained indoor location)
    // after that render sprites.
    if (world_graph->get_current_node()->in_overworld) {
      for (auto& m : maps) if (m->current_mID != null_map_id) m->render();
    }
    else {
      maps[0]->render();
    }
    for (auto& s : sprites) s->render();
    // TODO: render grass, etc, things that need to be rendered over the player
	}

  void Manager::clean()
  {
    for (auto& s: sprites) delete s;
    for (auto& m: maps)    delete m;
    for (auto& s: shaders) delete s;
  }

  void Manager::refresh()
  {
    using std::remove_if, std::begin, std::end;
    auto is_not_active = [](const Sprite* sprite) { return !sprite->is_active; };
		sprites.erase( remove_if(begin(sprites),end(sprites),is_not_active), end(sprites) );
	}
}