#include "text.h"
#include <SDL_image.h>
#include "scene.h"

Text_Sprite::Text_Sprite(Scene::Manager& man, Shader& s) 
: Sprite(0.0f, 0.0f, 0.002f, *man.camera_controller, s)
{
  init_buffers();
  init_textures();
//   glUseProgram(shader.handle);
//   shader.set("frame_ID", idle_d);
//   glUseProgram(0);
}
Text_Sprite::~Text_Sprite()
{
    delete char_buffer;
}

void Text_Sprite::init_textures()
{
  std::vector<const char*> text_sprite_paths=
  { // note: order matches ascii starting at space
    "assets/char/sp.png",
    "assets/char/exc.png",
    "assets/char/quo.png",
    "assets/char/hash.png",
    "assets/char/dol.png",
    "assets/char/prc.png",
    "assets/char/amp.png",
    "assets/char/apo.png",
    "assets/char/lp.png",
    "assets/char/rp.png",
    "assets/char/ast.png",
    "assets/char/com.png",
    "assets/char/dash.png",
    "assets/char/dot.png",
    "assets/char/fs.png",
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
  typedef struct {
    size_t
      width_pixels   = 9,
      height_pixels  = width_pixels,
      count          = 94,
      bytes_per_pixel= 4,
      bytes_per_frame= width_pixels * height_pixels * bytes_per_pixel;
  } Texture_Info;
  constexpr Texture_Info tex_info;
  SDL_Surface * loaded_image= nullptr;
  GLubyte * p_pixel_data= new GLubyte[tex_info.bytes_per_frame * tex_info.count];
  unsigned offset=0;
  std::cout << text_sprite_paths.size() << '\n';
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, & this->opengl_texture_ID);
  glBindTexture(GL_TEXTURE_2D_ARRAY, this->opengl_texture_ID);
  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, tex_info.width_pixels, tex_info.height_pixels, tex_info.count); // allocate GPU memory
  for (const char * file_path : text_sprite_paths)
  { // Load each frame's pixel data from file into a GLubyte array
    loaded_image = IMG_Load(file_path);
    if (loaded_image == nullptr)
      Debug::log_error_abort(Debug::text,std::hex,this,std::dec,": failed to load an animation texture:",file_path);
    memcpy(p_pixel_data + offset, loaded_image->pixels, tex_info.bytes_per_frame);
    SDL_FreeSurface(loaded_image);
    offset += tex_info.bytes_per_frame;
  }
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, tex_info.width_pixels, tex_info.height_pixels, tex_info.count, GL_RGBA, GL_UNSIGNED_BYTE, p_pixel_data); // send pixel data to GPU memory
  delete p_pixel_data; // free pixel data from heap

  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Text_Sprite::init_buffers()
{
  float vbo_data[]=
  {
   -0.0f, 1.0f, 0.0f,      0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,      1.0f, 0.0f,
    1.0f,-0.0f, 0.0f,      1.0f, 1.0f,
   -0.0f,-0.0f, 0.0f,      0.0f, 1.0f
  };
  unsigned ebo_data[]=
  {
    0, 1, 2,
    2, 3, 0
  };

  constexpr GLsizei buffer_size = 36;
  char_buffer= new GLint[buffer_size];
  for (int i = 0; i < buffer_size; ++i)
    char_buffer[i] = i + 1;

  this->n_verts= 6;
  glGenVertexArrays(1, &this->vao);
  glGenBuffers(1, &this->ebo);
  glGenBuffers(1, &this->vbo);
  glGenBuffers(1, &this->ssbo);
  Debug::log_from(Debug::text,"specifying buffers");
  glBindVertexArray(this->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ebo_data), ebo_data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_data), vbo_data, GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->ssbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, buffer_size*sizeof(int), char_buffer, GL_DYNAMIC_DRAW); // 4 bytes per int, 36 characters in the buffer.

  glEnableVertexArrayAttrib(this->vao, 0);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 20, (void*)0);
  glEnableVertexArrayAttrib(this->vao, 1);
  glVertexAttribPointer(1, 2, GL_FLOAT, false, 20, (void*)12);
}

void Text_Sprite::update(float dt, const uint8_t * key_states)
{
  this->mv= cam.get_WorldToView_Matrix() * this->model;
}

void Text_Sprite::render()
{
  glUseProgram(shader.handle);
  shader.set("mvp", cam.get_ViewToProjection_Matrix() * mv);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, this->opengl_texture_ID);
  glBindVertexArray(vao);
  glDrawElementsInstanced(GL_TRIANGLES, n_verts, GL_UNSIGNED_INT, 0, 36);
  glBindVertexArray(0);
  glUseProgram(0);
}