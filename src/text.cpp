#include "text.h"
#include "scene.h"
#include "shader.h"
#include "texture.h"



constexpr GLsizei char_buffer_size = 64; // maximum number of characters in the gl character buffer



using namespace Debug;



typedef std::vector<GLuint*> Buffer_List;
// prototype for initializing many buffer objects at once, may create buffer.h for functions like this, similar to how texture.h works
void generate_buffers(Buffer_List list)
{
  glGenBuffers(list.size(), *list.data());
}



// Construct Text_Manager. 
// This also constructs two derived sprite classes:
// Text_Box and Text_Lines
Text_Manager::Text_Manager(Scene::Manager &man)
: scene_manager(man),
  text_buffer(),
  text_lines(man.new_shader("shader/TextLines.glsl")),
  text_box(man.new_shader("shader/TextBox.glsl"))
{
  // participate in pokegl object tracking
  obj_identify(text,alloc,this,"Text-Manager");

  // allocate and zero the char buffer (aka fill it with spaces)
  char_buffer= new GLint[char_buffer_size];
  for (int i = 0; i < char_buffer_size; ++i)
    char_buffer[i] = 0;

  // character text lines shader vertex buffer
  float vbl_data[]= // non-negative unit square
  {//x     y
     0.f, +1.f, // top-left
    +1.f, +1.f, // top-right
    +1.f,  0.f, // bot-right
     0.f,  0.f  // bot-left
  };

  // box background shader vertex buffer
  float vbb_data[]= // centered unit square with texture coordinates per vertex
  {// x     y    u    v
    -.5f, +.5f,  0.f, 0.f,
    +.5f, +.5f,  1.f, 0.f,
    +.5f, -.5f,  1.f, 1.f,
    -.5f, -.5f,  0.f, 1.f
  };

  // standard quad element buffer
  unsigned eb_data[]=
  {
    0, 1, 2,
    2, 3, 0
  };

// Begin initializing vertex array state and creating buffers

  // generate opengl objects
  glGenVertexArrays(1, &va);
  generate_buffers({ &eb, &vbl, &vbb, &ssb });

  // bind the text managers vertex array object
  glBindVertexArray(va);

  // permanently set this as the index buffer for the currently bound vertex array
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(eb_data), eb_data, GL_STATIC_DRAW);

  // create the vertex buffer for text lines
  glBindBuffer(GL_ARRAY_BUFFER, vbl);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vbl_data), vbl_data, GL_STATIC_DRAW);

  // DSA functions: affect currently bound Vertex Array Obj
  glEnableVertexAttribArray(0);
  glBindVertexBuffer(0, vbl, 0, 8);
  glVertexAttribFormat(0, 4, GL_FLOAT, false, 0);

  // create the vertex buffer for text box
  glBindBuffer(GL_ARRAY_BUFFER, vbb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vbb_data), vbb_data, GL_STATIC_DRAW);

  // DSA functions: affect currently bound Vertex Array Obj
  glEnableVertexAttribArray(1);
  glBindVertexBuffer(1, vbb, 0, 16);
  glVertexAttribFormat(1, 4, GL_FLOAT, false, 0);

  // create the shader storage buffer object which will act as the string data
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssb);
  glBufferData(GL_SHADER_STORAGE_BUFFER, char_buffer_size * sizeof(int), char_buffer, GL_DYNAMIC_DRAW);

  // finished modifying state
  glBindVertexArray(0);

  // initialize text-lines texture object
  std::vector<const char*> char_sprite_paths= { // note: order matches ascii starting at space
    "assets/char/sp.png",   // 0
    "assets/char/exc.png",  // 1
    "assets/char/quo.png",  // 2
    "assets/char/hash.png", // 3
    "assets/char/dol.png",  // 4
    "assets/char/prc.png",  // 5
    "assets/char/amp.png",  // 6
    "assets/char/apo.png",  // 7
    "assets/char/lp.png",   // 8
    "assets/char/rp.png",   // 9
    "assets/char/ast.png",  // 10
    "assets/char/add.png",  // 11
    "assets/char/com.png",  // 12
    "assets/char/dash.png", // 13
    "assets/char/dot.png",  // 14
    "assets/char/fs.png",   // 15
    "assets/char/0.png",
    "assets/char/1.png",
    "assets/char/2.png",
    "assets/char/3.png",
    "assets/char/4.png",
    "assets/char/5.png",
    "assets/char/6.png",
    "assets/char/7.png",
    "assets/char/8.png",
    "assets/char/9.png",
    "assets/char/col.png",
    "assets/char/semi-col.png",
    "assets/char/la.png",
    "assets/char/eq.png",
    "assets/char/ra.png",
    "assets/char/qst.png",
    "assets/char/at.png",
    "assets/char/+a.png",
    "assets/char/+b.png",
    "assets/char/+c.png",
    "assets/char/+d.png",
    "assets/char/+e.png",
    "assets/char/+f.png",
    "assets/char/+g.png",
    "assets/char/+h.png",
    "assets/char/+i.png",
    "assets/char/+j.png",
    "assets/char/+k.png",
    "assets/char/+l.png",
    "assets/char/+m.png",
    "assets/char/+n.png",
    "assets/char/+o.png",
    "assets/char/+p.png",
    "assets/char/+q.png",
    "assets/char/+r.png",
    "assets/char/+s.png",
    "assets/char/+t.png",
    "assets/char/+u.png",
    "assets/char/+v.png",
    "assets/char/+w.png",
    "assets/char/+x.png",
    "assets/char/+y.png",
    "assets/char/+z.png",
    "assets/char/lsq.png",
    "assets/char/bs.png",
    "assets/char/rsq.png",
    "assets/char/crt.png",
    "assets/char/scr.png",
    "assets/char/btk.png",
    "assets/char/a.png",
    "assets/char/b.png",
    "assets/char/c.png",
    "assets/char/d.png",
    "assets/char/e.png",
    "assets/char/f.png",
    "assets/char/g.png",
    "assets/char/h.png",
    "assets/char/i.png",
    "assets/char/j.png",
    "assets/char/k.png",
    "assets/char/l.png",
    "assets/char/m.png",
    "assets/char/n.png",
    "assets/char/o.png",
    "assets/char/p.png",
    "assets/char/q.png",
    "assets/char/r.png",
    "assets/char/s.png",
    "assets/char/t.png",
    "assets/char/u.png",
    "assets/char/v.png",
    "assets/char/w.png",
    "assets/char/x.png",
    "assets/char/y.png",
    "assets/char/z.png",
    "assets/char/lc.png",
    "assets/char/bar.png",
    "assets/char/rc.png",
    "assets/char/tld.png"
  };
  generate_texture(GL_TEXTURE_2D_ARRAY, &tl);
  int w, h, count = char_sprite_paths.size();
  GLubyte * image_data= load_textures(char_sprite_paths, text, this, &w, &h, 4);
  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, w, h, count); // allocate GPU memory
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, w, h, count, GL_RGBA, GL_UNSIGNED_BYTE, image_data); // send pixel data to GPU memory
  delete image_data;

  // initialize text-box texture object
  generate_texture(GL_TEXTURE_2D, &tb);
  image_data= load_textures({"assets/textbox.png"}, textbox, this, &w, &h, 4);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
  glUseProgram(text_box.handle);
  text_box.set("textbox_texture",0); // assumes GL_TEXTURE0 is the active texture slot
  glUseProgram(0);
  delete image_data;
}



Text_Manager::~Text_Manager()
{
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
  update_gl_char_buffer( text_buffer );
  // set state flag
  has_text = true;
}



void Text_Manager::update(float t, const Uint8 * key_states)
{
  if (!has_text && key_states[SDL_SCANCODE_H]) {
    set_text("Hello, World!");
  }
  // if textlines has reached the end of string {
  //   show a arrow and wait for input
  //   set state: waiting for input
  // }
  if (text_index < eos_index) {
    ++text_index;
  }
}



// render textbox and up to the current index instances of the character drawing shader.
void Text_Manager::render()
{
  if (!has_text)
    return;
  glBindVertexArray(va);
  glUseProgram(text_box.handle);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tb);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  if(text_index > 0) {
    glUseProgram(text_lines.handle);
    glBindTexture(GL_TEXTURE_2D_ARRAY, tl);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, text_index);
  }
  glBindVertexArray(0);
  glUseProgram(0);
}



// overwrites the gl char buffer over the new string and updates the 
// state variables of this textlines object: 
// end_of_string_index and the current_text_index 
// this automatically starts a new animation by setting text index to 0.
void Text_Manager::update_gl_char_buffer(std::string& new_data)
{
  // reset state variables
  eos_index= new_data.size();
  text_index= 0;

  // update the actual buffer data
  for (int i = 0; i < new_data.size(); ++i)
    char_buffer[i] = new_data.at(i) - 32;

  // send new string data to GPU (likely not the whole buffer)
  glBindVertexArray(va);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssb);
  glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, eos_index * sizeof(int), char_buffer);
  glBindVertexArray(0);
}