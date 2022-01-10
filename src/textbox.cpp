#include "textbox.h"
#include <SDL_image.h>
#include "scene.h"

Text_Box_Sprite::Text_Box_Sprite(Scene::Manager& man, Shader& s) 
: Sprite(0.0f, 0.0f, 0.002f, *man.camera_controller, s)
{
  init_buffers();
  init_texture();
}

Text_Box_Sprite::~Text_Box_Sprite()
{
    delete char_buffer;
}

void Text_Box_Sprite::init_texture()
{
  using namespace Debug;
  SDL_Surface * image= IMG_Load("assets/textbox.png");
  if (image == nullptr) log_error_abort(text,"textbox could not load texture");
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &t);
  glBindTexture(GL_TEXTURE_2D, t);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glUseProgram(shader.handle);
  shader.set("textbox_texture",0);
  glUseProgram(0);
}

void Text_Box_Sprite::init_buffers()
{
  float vb_data[]=
  {// (position:vec3)      (texture coordinates:vec2)
   -1.0f, -0.2f, 0.0f,      0.0f, 0.0f,
   +1.0f, -0.2f, 0.0f,      1.0f, 0.0f,
   +1.0f, -1.0f, 0.0f,      1.0f, 1.0f,
   -1.0f, -1.0f, 0.0f,      0.0f, 1.0f
  };
  unsigned eb_data[]=
  {
    0, 1, 2,
    2, 3, 0
  };
  n_verts= 6;
  glGenVertexArrays(1, &va);
  glGenBuffers(1, &eb);
  glGenBuffers(1, &vb);
  glBindVertexArray(va);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(eb_data), eb_data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vb_data), vb_data, GL_STATIC_DRAW);
  glEnableVertexArrayAttrib(va, 0);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 20, (void*)0);
  glEnableVertexArrayAttrib(va, 1);
  glVertexAttribPointer(1, 2, GL_FLOAT, false, 20, (void*)12);
  glBindVertexArray(0);
}

void Text_Box_Sprite::update(float dt, const uint8_t * key_states)
{
  SDL_Event e;
}

void Text_Box_Sprite::render()
{
  glUseProgram(shader.handle);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, t);
  glBindVertexArray(va);
  glDrawElements(GL_TRIANGLES, n_verts, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}