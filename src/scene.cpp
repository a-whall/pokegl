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
    sprites.push_back(new Player(*this, s));
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

  void Manager::update_map(Map_ID_enum mID)
  {
    World_Node * wnode;
    Map_ID_enum m;
    maps[0]->change(mID);
    world_graph->set_current_node(mID);
    World_Node * cnode = world_graph->get_current_node();

    if (cnode->in_overworld) {
      Debug::log_from(Debug::scene,"cnode in overworld");
      if ((m=cnode->up()) != null_map_id) {
        Debug::log_from(Debug::scene,"updating maps::up");
        wnode = world_graph->get_node(m);
        maps[1]->change(m);
        maps[1]->shift_vertex_coords_by_offset(0,maps[0]->h_tiles);
        maps[1]->is_visible= true;
      }
      if ((m=cnode->left()) != null_map_id) {
        Debug::log_from(Debug::scene,"updating maps::left");
        wnode = world_graph->get_node(m);
        maps[2]->change(m);
        maps[2]->shift_vertex_coords_by_offset(-maps[2]->w_tiles,0);
      }
      if ((m=cnode->down()) != null_map_id) {
        Debug::log_from(Debug::scene,"updating maps::down");
        wnode = world_graph->get_node(m);
        maps[3]->change(m);
        maps[3]->shift_vertex_coords_by_offset(0,-maps[3]->h_tiles);
      }
      if ((m=cnode->right()) != null_map_id) {
        Debug::log_from(Debug::scene,"updating maps::right");
        wnode = world_graph->get_node(m);
        maps[4]->change(m);
        maps[4]->shift_vertex_coords_by_offset(maps[0]->w_tiles,0);
      }
    //overworld_chunks.p_left_map = &scene_manager.new_map(route_29, *map_shader);
    //overworld_chunks.p_left_map->shift_vertex_coords_by_offset(-overworld_chunks.p_left_map->w_tiles, 0);
    //overworld_chunks.p_left_map->is_visible= false;
    }
    else {// set neighbors invisible
      Debug::log_from(Debug::scene,"cnode is not in overworld");
      maps[1]->is_visible = maps[2]->is_visible = maps[3]->is_visible
      = maps[4]->is_visible = false;
    }
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
    using std::remove_if;
    using std::begin;
    using std::end;
    auto is_not_active = [](const Sprite* sprite) { return !sprite->is_active; };
		sprites.erase( remove_if(begin(sprites),end(sprites),is_not_active), end(sprites) );
	}
}