#pragma once
#include <string>    // std::string
#include <SDL.h>     // Uint8
#include <GL/glew.h> // GLuint
#include <queue>


// All text will be rendered and updated by a single object: Text_Manager.

namespace Scene { class Manager; }

class Shader;

//  state {
//    dialog,
//    menu,
//    answer
//  }


typedef std::string Text_Buffer;



class Text_Manager
{
  Scene::Manager &scene_manager;             // reference to the owner of this Text_Manager
  Text_Buffer text_buffer;                   // string data
  Shader &text_box, &text_lines;             // 2 shaders
  GLuint va, val, vbb, vbl, eb, ssb, tb, tl; // opengl object handles
  int * char_buffer;                         // opengl string buffer

public:

  int si, ci, fi, len;
  bool set_baseID=false;
  bool dialog=false;
  bool menu=false;
  bool yesno=false;
  std::queue<SDL_Keysym> key_downs;

  Text_Manager(Scene::Manager &man);
  ~Text_Manager();

  void update_gl_char_buffer();
  void set_dialog(std::string&&);
  void update(float, const Uint8 *);
  void render();

  void print_state();

  // dialog may be in the middle of animation
  // *mouse-click-event* (for example)
  // text-manager could have it's own event queue
  // to which the scene_manager can forward events of interest.
  // then on update, it will process those events.
};