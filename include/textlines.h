#pragma once
#include "sprite.h"

namespace Scene { class Manager; }

class Text_Sprite : public Sprite
{
  GLuint va, vb, eb, ssb, t;
  int * char_buffer;
  int data_size, text_counter = 0;
  std::string data="";
  bool writing= false;

  // eos flag for state tracking.
public:
  Text_Sprite(Scene::Manager& ,Shader&);
  ~Text_Sprite();

  void set_text(const char *);
  void init_textures();
  void init_buffers() override;
  void update(float,const Uint8 *) override;
  void render() override;
};