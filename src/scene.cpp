#include <algorithm>
#include "scene.h"
#include "gtc/matrix_transform.hpp"
#include "world.h"

using namespace Debug;

namespace Scene
{
  void Manager::print_objects()
  {
    for (Player * p : sprites) p->identify();
    for (auto& m : maps) m.second->identify();
    text_manager->textbox->identify();
    text_manager->textlines->identify();
  }



  void Manager::init_camera(int x, int y)
  {
    float aspect_ratio (x / y);
    camera_controller = new Camera(vec3(0, 0, 5), vec3(0, 0, -1), aspect_ratio);
  }



  Player& Manager::new_player(Shader& s)
  {
    Camera& c = *camera_controller;
    sprites.push_back(new Player(*this, s));
    return *sprites.back();
  }



  void Manager::init_text()
  {
    text_manager= new Text_Manager(*this);
  }



  void Manager::init_maps()
  {
    Shader& s = new_shader("shader/MapSprite.glsl");
    World_Node * active_wnode = world_graph->get_current_node();
    if (active_wnode == nullptr)
      log_error_abort(scene,"manager expected the world_graph to have a current world node");
    Camera& c = *this->camera_controller;
    maps[middle] = new Map(active_wnode->mID, c, s);
    maps[up]     = new Map(null_map_id, c, s);
    maps[left]   = new Map(null_map_id, c, s);
    maps[down]   = new Map(null_map_id, c, s);
    maps[right]  = new Map(null_map_id, c, s);
  }



  void Manager::update_map(Map_ID_enum mID)
  {
    // This is a helper function that exchanges the map pointers to 3 of the 5 total map objects. This way, for example, when walking left onto a new map zone the maps which are already loaded on the gpu can be reused.
    // Note: I was writing this and trying to get it to work and these random translations by (0,0) worked mysteriously. I was just testing, I'm not sure why the same translation for all maps ...just works...
    auto cascade_maps = [&](Map_Orientation i1, Map_Orientation i2) {
      Map * tmp = maps[i1];
      maps[i1] = maps[middle];
      maps[i1]->translate(0,0);
      maps[i1]->is_visible=true;
      maps[middle] = maps[i2];
      maps[middle]->translate(0,0);
      maps[middle]->is_visible=true;
      maps[i2] = tmp;
      maps[i2]->change(mID);
      maps[i2]->translate(0,0);
      maps[i2]->is_visible=true;
    };

    World_Node * wnode = world_graph->get_current_node();

    if      (wnode->up() == mID) cascade_maps(down,up);
    else if (wnode->left() == mID) cascade_maps(right,left);
    else if (wnode->down() == mID) cascade_maps(up,down);
    else if (wnode->right() == mID) cascade_maps(left,right);
    else maps[middle]->change(mID); // this will naturally happen for non-overworld maps.

    World_Node * cnode = world_graph->set_current_node(mID);

    // check to update music
    if (get_area_track(cnode->mID) != get_area_track(wnode->mID))
      sound.play_song(get_area_track(cnode->mID));

    Map_ID_enum m; // temp value for the ifs below

    if (cnode->in_overworld) {
      log_from(scene,"cnode in overworld");
      if ((m=cnode->up()) != null_map_id) {        // if the new up map has null id according to world graph:
        //std::cout << "maps[up].id: " << maps[up]->current_mID << " current_node.up().id: " << m << '\n';
        if (maps[up]->current_mID != m)               // if the up map hasn't already been swapped by cascade_maps:
          maps[up]->change(m);                           // load the map
        maps[up]->translate(0,maps[middle]->h_tiles); // translate the map sprite appropriately
        maps[up]->update(0,nullptr);                  // update the map object so that the correct matrix is used for rendering
        maps[up]->is_visible= true;                   // set the map to be visible
      }
      else maps[up]->is_visible= false;         // else turn the up maps visibility off
      if ((m=cnode->left()) != null_map_id) {  // repeat
        //std::cout << "maps[left].id: " << maps[left]->current_mID << " current_node.left().id: " << m << '\n';
        if (maps[left]->current_mID != m)
          maps[left]->change(m);
        maps[left]->translate(-maps[left]->w_tiles,0);
        maps[left]->update(0, nullptr);
        maps[left]->is_visible= true;
      }
      else maps[left]->is_visible= false;
      if ((m=cnode->down()) != null_map_id) {
        //std::cout << "maps[down].id: " << maps[down]->current_mID << " current_node.down().id: " << m << '\n';
        if (maps[down]->current_mID != m)
          maps[down]->change(m);
        maps[down]->translate(0,-maps[left]->h_tiles);
        maps[down]->update(0,nullptr);
        maps[down]->is_visible= true;
      }
      else maps[down]->is_visible= false;
      if ((m=cnode->right()) != null_map_id) {
        //std::cout << "maps[right].id: " << maps[right]->current_mID << " current_node.right().id: " << m << '\n';
        if (maps[right]->current_mID != m)
          maps[right]->change(m);
        maps[right]->translate(maps[middle]->w_tiles,0);
        maps[right]->update(0,nullptr);
        maps[right]->is_visible= true;
      }
      else maps[right]->is_visible= false;
    }
    else {// set neighbors invisible
      log_from(scene,"cnode is not in overworld");
      maps[middle]->change(mID);
      maps[up]->is_visible= maps[left]->is_visible= maps[down]->is_visible= maps[right]->is_visible= false;
    }
  }



  Shader& Manager::new_shader(const char* file_name)
  {
    shaders.push_back(new Shader(glCreateProgram()));
    compile(shaders.back()->handle, file_name);
    shaders.back()->log_program_resources();
    return *shaders.back();
  }



  void Manager::update(float t)
  {
    for (auto& s : sprites) s->update(t, key_states);
    for (auto& m : maps)    m.second->update(t, key_states);
    text_manager->update(t, key_states);
  }



  void Manager::render()
  {
    // if the active world node is flagged as being 'in_overworld' then render all maps
    // otherwise print only the main map (which would be a self contained indoor location)
    // after that render sprites.
    if (world_graph->get_current_node()->in_overworld) {
      for (auto& m : maps) if (m.second->current_mID != null_map_id) m.second->render();
    }
    else {
      maps[middle]->render();
    }
    for (auto& s : sprites) s->render();
    // TODO: render grass, etc, things that need to be rendered over the player
    text_manager->render();
  }



  void Manager::clean()
  {
    delete world_graph;
    for (auto& s: sprites) delete s;
    for (auto& m: maps)    delete m.second;
    for (auto& s: shaders) delete s;
    delete text_manager;
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