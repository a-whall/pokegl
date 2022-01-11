#include "text.h"
#include "scene.h"


Text_Manager::Text_Manager(Scene::Manager &man)
: scene_manager(man)
{
  text= new Text_Sprite(man, man.new_shader("shader/Text.glsl"));
  textbox = new Text_Box_Sprite(man, man.new_shader("shader/TextBox.glsl"));
}

Text_Manager::~Text_Manager()
{
  delete text;
  delete textbox;
}