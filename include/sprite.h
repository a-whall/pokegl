#pragma once
#include "SDL.h"
#include "camera.h"
#include "shader.h"

class Sprite
{
protected:

	int n_verts = 0;
	mat4 model;
	mat4 mv;

	Camera& cam;
	Shader& shader;

public:

	bool is_active = true;
	Sprite(float, float, float, Camera&, Shader&);

  virtual ~Sprite();

	virtual void init_buffers() = 0;
	virtual void update(float, const Uint8*) = 0;
	virtual void render() = 0;

	virtual mat4 get_model() final;
	virtual void set_model(mat4 m) final;
};