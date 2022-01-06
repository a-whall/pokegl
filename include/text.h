#pragma once
#include "sprite.h"

namespace Scene {
  class Manager;
}

class Text_Sprite : public Sprite
{
  GLuint vao, vbo, ebo, ssbo, opengl_texture_ID;
  char char_buffer[36];
public:
  Text_Sprite(Scene::Manager& ,Shader&);
  ~Text_Sprite();

  void init_textures();
  void init_buffers() override;
  void update(float,const Uint8 *) override;
  void render() override;
};