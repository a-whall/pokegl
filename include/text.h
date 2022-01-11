#pragma once
#include "textlines.h"
#include "textbox.h"

namespace Scene { class Manager; }

class Text_Manager
{
  Scene::Manager &scene_manager;

  // DATA
  // 

public:

  Text_Sprite *text;
  Text_Box_Sprite *textbox;

  Text_Manager(Scene::Manager &man);
  ~Text_Manager();
};