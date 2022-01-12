#pragma once
#include "textlines.h"
#include "textbox.h"

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