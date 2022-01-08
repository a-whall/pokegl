#include "sprite.h"
#include "debug.h"

Sprite::Sprite(float x, float y, float z, Camera &c, Shader &s)
: cam(c), shader(s)
{
  model = translate(mat4(1.0f), vec3(x,y,z));
  mv = mat4(1.0f);
}

Sprite::~Sprite()
{
  // empty
}

void Sprite::identify() { Debug::log_from(Debug::sprite,std::hex,this,std::dec); };
mat4 Sprite::get_model()       { return model; };
void Sprite::set_model(mat4 m) { model = m; };