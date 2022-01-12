#include "text.h"
#include "scene.h"



Text_Manager::Text_Manager(Scene::Manager &man)
: scene_manager(man)
{
  textbox = new Text_Box_Sprite(man, man.new_shader("shader/TextBox.glsl"));
  textlines= new Text_Sprite(man, man.new_shader("shader/Text.glsl"));
}



Text_Manager::~Text_Manager()
{
  delete textlines;
  delete textbox;
}


// Text Manager may be seen as the state of text rendering in PokeGL. 
// This update function will update both scene objects textbox and textlines, in that order.
void Text_Manager::update(float t, const Uint8 * key_states)
{
  textbox->update(t, key_states);
  textlines->update(t, key_states);
}



void Text_Manager::render()
{
  textbox->render();
  textlines->render();
}