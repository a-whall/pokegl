#include "textlines.h"
#include "text.h"
#include <SDL_image.h>
#include "scene.h"
#include "texture.h"



using namespace Debug;



Text_Lines::Text_Lines(Scene::Manager& man, Shader& s) 
: Sprite(0.0f, 0.0f, 0.002f, *man.camera_controller, s)
{
  obj_identify(text,alloc,this,"Text-Lines");
  init_buffers();
  init_textures();
}



Text_Lines::~Text_Lines()
{
  delete char_buffer;
  obj_identify(text,dealloc,this,"Text-Lines");
}



void Text_Lines::init_textures()
{
  std::vector<const char*> char_sprite_paths=
  { // note: order matches ascii starting at space
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
  generate_texture(GL_TEXTURE_2D_ARRAY, &t);
  int w, h, count = char_sprite_paths.size();
  GLubyte * image_data= load_textures(char_sprite_paths, text, this, &w, &h, 4);
  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, w, h, count); // allocate GPU memory
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, w, h, count, GL_RGBA, GL_UNSIGNED_BYTE, image_data); // send pixel data to GPU memory
  delete image_data;
}



void Text_Lines::init_buffers()
{
  float vb_data[]= // non-negative unit square
  {
    0.f, +1.f,
   +1.f, +1.f,
   +1.f,  0.f,
    0.f,  0.f
  };
  unsigned eb_data[]= // standard quad element buffer
  {
    0, 1, 2,
    2, 3, 0
  };

  char_buffer= new GLint[char_buffer_size];
  for (int i = 0; i < char_buffer_size; ++i)
    char_buffer[i] = 0;

  n_verts= 6;

  glGenVertexArrays(1, &va);
  glGenBuffers(1, &eb);
  glGenBuffers(1, &vb);
  glGenBuffers(1, &ssb);

  glBindVertexArray(va);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(eb_data), eb_data, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vb_data), vb_data, GL_STATIC_DRAW);

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssb);
  glBufferData(GL_SHADER_STORAGE_BUFFER, char_buffer_size * sizeof(int), char_buffer, GL_DYNAMIC_DRAW);

  glEnableVertexArrayAttrib(va, 0);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 8, (void*)0);

  glBindVertexArray(0);
}



// The Text_Manager will send this function the new string for processing. 
// It overwrites the gl char buffer over the new string and updates the 
// state variables of this textlines object: 
// end_of_string_index and the current_text_index 
// this automatically starts a new animation.
void Text_Lines::update_gl_char_buffer(std::string new_data)
{
  // reset state variables
  eos_index= new_data.size();
  text_index= 0;

  // update the actual buffer data
  for (int i = 0; i < new_data.size(); ++i)
    char_buffer[i] = new_data.at(i) - 32;

  // send new buffer data to GPU
  glBindVertexArray(va);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssb);
  glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, eos_index * sizeof(int), char_buffer);
  glBindVertexArray(0);
}



void Text_Lines::update(float dt, const uint8_t * key_states)
{
  // TODO: update text index using text speed setting. (modulo)
  // update c++ side member variables.
  if (text_index < eos_index) {
    ++text_index;
  }
}



void Text_Lines::render()
{
  if(text_index > 0) { 
    glUseProgram(shader.handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, t);
    glBindVertexArray(va);

    glDrawElementsInstanced(GL_TRIANGLES, n_verts, GL_UNSIGNED_INT, 0, text_index);

    glBindVertexArray(0);
    glUseProgram(0);
  }
}