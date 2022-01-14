#pragma once
#include <string>    // std::string
#include <SDL.h>     // Uint8
#include <GL/glew.h> // GLuint


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

  int eos_index;    // the size of the data string
  int text_index=0; // render this many instances of the text character shader.
  bool has_text=false;

  Text_Manager(Scene::Manager &man);
  ~Text_Manager();

  void update_gl_char_buffer(std::string&);
  void set_text(std::string&&);
  void update(float, const Uint8 *);
  void render();
};