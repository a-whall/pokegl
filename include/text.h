#pragma once
#include "textlines.h"
#include "textbox.h"

namespace Scene { class Manager; }

/*
  enum Text_enum {
    dialog,
    menu,
    answer
  }
*/

class Text_Manager
{
  Scene::Manager &scene_manager;

  // DATA
  // 

public:

  Text_Sprite *textlines;
  Text_Box_Sprite *textbox;

  Text_Manager(Scene::Manager &man);
  ~Text_Manager();

  void update(float, const Uint8 *);
  void render();
};