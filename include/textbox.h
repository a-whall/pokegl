#pragma once
#include "sprite.h"

namespace Scene { class Manager; } // forward declare for Manager reference in constructor


class Text_Box_Sprite
: public Sprite
{
  GLuint va, vb, eb, t;
  // eos flag for state tracking.
  int * char_buffer;

public:
  Text_Box_Sprite(Scene::Manager& ,Shader&);
  ~Text_Box_Sprite();

  void init_texture();
  void init_buffers() override;
  void update(float,const Uint8 *) override;
  void render() override;
};