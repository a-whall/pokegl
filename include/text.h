#pragma once
#include "textlines.h"
#include "textbox.h"


// The philosophy I am choosing is to have all text be rendered and updated by a single object. 
// This is the purpose of Text_Manager. To encapsulate everything to do with text.

namespace Scene { class Manager; }


//  state {
//    dialog,
//    menu,
//    answer
//  }


typedef std::string Text_Buffer;



class Text_Manager
{
  Scene::Manager &scene_manager; // reference to the owner of this Text_Manager
  Text_Buffer text_buffer; // data

public:

  bool has_text=false;

  Text_Lines *textlines;
  Text_Box *textbox;

  Text_Manager(Scene::Manager &man);
  ~Text_Manager();

  void set_text(std::string&&);
  void update(float, const Uint8 *);
  void render();
};