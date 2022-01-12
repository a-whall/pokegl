#include "text.h"
#include "scene.h"



using namespace Debug;



// Construct Text_Manager. 
// This also constructs two derived sprite classes:
// Text_Box and Text_Lines
Text_Manager::Text_Manager(Scene::Manager &man)
: scene_manager(man),
  text_buffer()
{
  obj_identify(text,alloc,this,"Text-Manager");
  textbox= new Text_Box(man, man.new_shader("shader/TextBox.glsl"));
  textlines= new Text_Lines(man, man.new_shader("shader/TextLines.glsl"));
}



Text_Manager::~Text_Manager()
{
  delete textbox;
  delete textlines;
  obj_identify(text,dealloc,this,"Text-Manager");
}



// This will act as the standard interface for the rest of the program to interact 
// with from their references to the scene manager. It updates the dialog text buffer 
// sets the flag has_text to change the state of Text_Manager.
void Text_Manager::set_text(std::string&& msg)
{
  // has_text boolean: basically an is_writing statecheck
  if (has_text) {
    log_error_from(text,"no message queue has been implemented\n\t "
      "Text_Manager::set_text() is ignoring a message request.\n");
    return;
  }
  // update text data stored in this object
  text_buffer = std::forward<std::string>(msg);
  // update the actual opengl buffer which stores dialog text
  textlines->update_gl_char_buffer( text_buffer );
  // set state flag
  has_text = true;
}



// Text Manager may be seen as the state of text rendering in PokeGL. 
// This update function will update both scene objects textbox and textlines, in that order.
void Text_Manager::update(float t, const Uint8 * key_states)
{
  if (!has_text && key_states[SDL_SCANCODE_H]) {
    set_text("Hello, World!");
    // update state.
  }
  // if textlines has reached the end of string {
  //   show a triangle and wait for input
  //   set state: waiting for input
  // }
  textbox->update(t, key_states);
  textlines->update(t, key_states);
}



void Text_Manager::render()
{
  if (has_text) {
    textbox->render();
    textlines->render();
  }
}