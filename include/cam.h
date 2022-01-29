#pragma once
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

using glm::vec2;
using glm::vec3;
using glm::mat4;

constexpr vec3 world_up_vector = vec3(0, 1, 0); // Y axis is considered world up vector

struct Camera
{
  vec3 pos;
  mat4 view;
  mat4 proj;

  Camera();
  ~Camera();

  void move(vec2);
  void set_Position(vec2);
};