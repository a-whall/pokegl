#pragma once
#include "sprite.h"



constexpr GLsizei char_buffer_size = 64; // maximum number of characters in the gl character buffer



namespace Scene { class Manager; }



class Text_Lines : public Sprite
{
  GLuint va, vb, eb, ssb, t;
  int * char_buffer;

public:

  int eos_index; // the size of the data string
  int text_index=0; // render this many instances of the text character shader.

  Text_Lines(Scene::Manager& ,Shader&);
  ~Text_Lines();

  void set_text(const char *);
  void init_textures();
  void update_gl_char_buffer(std::string);

  void init_buffers() override;
  void update(float,const Uint8 *) override;
  void render() override;
};