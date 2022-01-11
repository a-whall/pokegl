#include "textbox.h"
#include <SDL_image.h>
#include "scene.h"
#include "texture.h"
using namespace Debug;



Text_Box_Sprite::Text_Box_Sprite(Scene::Manager& man, Shader& s) 
: Sprite(0.0f, 0.0f, 0.0f, *man.camera_controller, s)
{
  init_buffers();
  init_texture();
}



Text_Box_Sprite::~Text_Box_Sprite()
{
}



void Text_Box_Sprite::init_texture()
{
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &t);
  glBindTexture(GL_TEXTURE_2D, t);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  int w, h;
  GLubyte * image_data= load_textures({"assets/textbox.png"}, textbox, this, &w, &h, 4);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
  glUseProgram(shader.handle);
  shader.set("textbox_texture",0);
  glUseProgram(0);
  delete image_data;
}



void Text_Box_Sprite::init_buffers()
{
  float vb_data[]=
  {// (position:vec2)  (texture coordinates:vec2)
   -.5f, +.5f,       0.f, 0.f,
   +.5f, +.5f,       1.f, 0.f,
   +.5f, -.5f,       1.f, 1.f,
   -.5f, -.5f,       0.f, 1.f
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
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 16, (void*)0);
  glEnableVertexArrayAttrib(va, 1);
  glVertexAttribPointer(1, 2, GL_FLOAT, false, 16, (void*)8);
  glBindVertexArray(0);
}



void Text_Box_Sprite::update(float dt, const uint8_t * key_states)
{
  SDL_Event e;
}



void Text_Box_Sprite::render()
{
  glUseProgram(shader.handle);
  // if (dialog_box)
  //   send dialog box Vertex-Position-Adjustment-Matrix
  // else if (menu_box)
  //   send menu box V-P-A-M
  // else if (answer box)
  //   send answer box V-P-A-M

  //glm::mat2 adj= glm::mat2(1.0f);
  //adj[0] = glm::vec2(2.0f, 0.0f);
  //adj[1] = glm::vec2(0.0f, 0.8f);
  //shader.set("adj",adj);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, t);
  glBindVertexArray(va);
  glDrawElements(GL_TRIANGLES, n_verts, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}