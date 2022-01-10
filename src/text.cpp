#include "text.h"
#include <SDL_image.h>
#include "scene.h"
#include "texture.h"

constexpr GLsizei char_buffer_size = 36; // number of characters in the character buffer

using namespace Debug;

Text_Sprite::Text_Sprite(Scene::Manager& man, Shader& s) 
: Sprite(0.0f, 0.0f, 0.002f, *man.camera_controller, s)
{
  obj_identify(text,alloc,this,"Text-Lines");
  init_buffers();
  init_textures();
}

Text_Sprite::~Text_Sprite()
{
  delete char_buffer;
  obj_identify(text,dealloc,this,"Text-Lines");
}

void Text_Sprite::init_textures()
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
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, & this->t);
  glBindTexture(GL_TEXTURE_2D_ARRAY, this->t);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  int w, h, count = char_sprite_paths.size();
  GLubyte * image_data= load_textures(char_sprite_paths, text, this, &w, &h, 4);
  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, w, h, count); // allocate GPU memory
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, w, h, count, GL_RGBA, GL_UNSIGNED_BYTE, image_data); // send pixel data to GPU memory
  delete image_data;
}

void Text_Sprite::init_buffers()
{
  float vb_data[]= // non-negative unit square, slightly towards camera in the z axis
  { // position x y z         tex coords
    0.0f, +1.0f, -0.1f,       0.0f, 0.0f,
   +1.0f, +1.0f, -0.1f,       1.0f, 0.0f,
   +1.0f,  0.0f, -0.1f,       1.0f, 1.0f,
    0.0f,  0.0f, -0.1f,       0.0f, 1.0f
  };
  unsigned eb_data[]= // standard quad element buffer
  {
    0, 1, 2,
    2, 3, 0
  };

  char_buffer= new GLint[char_buffer_size];
  for (int i = 0; i < char_buffer_size; ++i)
    char_buffer[i] = 0;

  this->n_verts= 6;

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
  glBufferData(GL_SHADER_STORAGE_BUFFER, char_buffer_size*sizeof(int), char_buffer, GL_DYNAMIC_DRAW); // 4 bytes per int, 36 characters in the buffer.

  glEnableVertexArrayAttrib(va, 0);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 20, (void*)0);
  glEnableVertexArrayAttrib(va, 1);
  glVertexAttribPointer(1, 2, GL_FLOAT, false, 20, (void*)12);

  glBindVertexArray(0);
}


void Text_Sprite::set_text(const char * message)
{
  if (writing) {
    log_error_from(text,"TODO: implement text message queue\n\t as it is this message will be ignored as one is text object is currently locked.\n");
    return;
  }
  // update text data stored in this object
  data = message;
  writing = true;
  text_counter = 0;
}


void Text_Sprite::update(float dt, const uint8_t * key_states)
{
  if (text_counter == 0 && key_states[SDL_SCANCODE_H])
    set_text("Hello, World!");

  if (writing) { // for now text counter will correspond directly to char_buffer index
    if (text_counter < data.length()) {
      int i (data.at(text_counter) - 32);
      log_from(text,"sending ",i);
      char_buffer[text_counter] = i;
      glBindVertexArray(va);
      glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssb);
      glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, char_buffer_size*sizeof(int), char_buffer);
      glBindVertexArray(0);
      text_counter++;
    } else {
      writing = false;
      text_counter = 0;
    }
  }
}

void Text_Sprite::render()
{
  glUseProgram(shader.handle);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, t);
  glBindVertexArray(va);

  glDrawElementsInstanced(GL_TRIANGLES, n_verts, GL_UNSIGNED_INT, 0, 36);

  glBindVertexArray(0);
  glUseProgram(0);
}