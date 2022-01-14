#pragma once
#include "sprite.h"

namespace Scene { class Manager; } // forward declare for Manager reference in constructor


class Text_Box
: public Sprite
{
  GLuint va, vb, eb, t;

public:
  Text_Box(Scene::Manager& ,Shader&);
  ~Text_Box();

  void update(float,const Uint8 *) override;
  void render() override;

  void set_menu_uniforms();

private:

  void init_texture();
  void init_buffers() override;
};