#pragma once
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

using glm::vec3;
using glm::mat4;

constexpr vec3 world_up_vector = vec3(0, 1, 0); // Y axis is considered world up vector

class Camera
{
  struct mouse_ui
  {
    bool firstMouse = true;
    float yaw = -90.0f;	// yaw is initialized to -90.0 degrees to face negative z direction (a yaw of 0 results in a direction vector pointing in the positive x)
    float pitch = 0.0f;
    float lastX = 0;
    float lastY = 0;
  };
  vec3 position;           // world pos
  mat4 world_to_view;      // view matrix
  mat4 view_to_projection; // projection matrix
  mouse_ui mui;            // mouse data struct

public:
  float speed = 2.5f;
  float look_sensitivity = 0.1f;
  float fov_degrees = 70.0f; // fov in Degrees
  vec3 forward_vector;
  vec3 right_vector;
  vec3 up_vector;

  Camera();
  Camera(vec3, vec3, float);

  void move(vec3);
  void handleMouseMotion(float, float);
  void handleMouseScroll(int);

  mat4 view();
  mat4 projection();
  vec3 get_position();
  void set_Position(vec3 pos);
  void set_ViewDirection(vec3 dir);
  void set_firstMouse(bool b);

private:
  void updateFOV(float scroll);
  void updateEulerAngles(float x, float y);
  void updateCameraVectors(float yaw, float pitch);
  void updateViewToProj();
};