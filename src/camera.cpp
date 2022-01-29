#include "camera.h"
#include <gtc/type_ptr.hpp>

Camera::Camera(): Camera(vec3(0, 0, 5), vec3(0, 0, -1), 1.25f){} // default camera-position, view-direction and aspect-ratio

Camera::Camera(vec3 pos, vec3 view_direction, float aspect)
{
  using namespace glm;
  look_sensitivity = 0.1f;
  position = pos;
  forward_vector = view_direction;
  world_to_view = lookAt(pos, normalize(pos - view_direction), world_up_vector);
  view_to_projection = perspective(radians(70.0f), aspect, 0.1f, 200.0f);
  right_vector = -normalize(cross(world_up_vector, forward_vector));
  up_vector = world_up_vector;
}

mat4 Camera::view()      { return lookAt(position, position + forward_vector, world_up_vector); }
mat4 Camera::projection() { return view_to_projection; }
vec3 Camera::get_position()                { return position; }

void Camera::set_Position(vec3 pos)      { position = pos; }
void Camera::set_ViewDirection(vec3 dir) { forward_vector = dir; }
void Camera::set_firstMouse(bool b)      { mui.firstMouse = b; }

void Camera::move(vec3 offset)
{
  position += offset;
  world_to_view = lookAt(position, normalize(position - forward_vector), world_up_vector);
}

void Camera::handleMouseMotion(float xpos, float ypos)
{
  if (mui.firstMouse) {
    mui.lastX = xpos;
    mui.lastY = ypos;
    mui.firstMouse = false;
  }
  else {
    float xOffset = xpos - mui.lastX;
    float yOffset = mui.lastY - ypos; // reversed since y-coordinates go from bottom to top
    mui.lastX = xpos;
    mui.lastY = ypos;
    updateEulerAngles(xOffset, yOffset);
    updateCameraVectors(mui.yaw, mui.pitch);
  }
}

void Camera::handleMouseScroll(int scroll)
{
  updateFOV(scroll);
  updateViewToProj();
}

void Camera::updateFOV(float scroll)
{
  fov_degrees -= scroll * .1; // decrease fov for scroll up
  if (fov_degrees < 1.0f)
    fov_degrees = 1.0f;
  else if (fov_degrees > 90.0f)
    fov_degrees = 90.0f;
}

void Camera::updateEulerAngles(float x, float y)
{
  x *= look_sensitivity;
  y *= look_sensitivity;
  mui.yaw += x;
  mui.pitch += y;
  if (mui.pitch > 89.0f)
    mui.pitch = 89.0f;
  if (mui.pitch < -89.0f)
    mui.pitch = -89.0f;
}

void Camera::updateCameraVectors(float yaw, float pitch)
{
  using namespace glm;
  forward_vector.x = cos(radians(yaw)) * cos(radians(pitch));
  forward_vector.y = sin(radians(pitch));
  forward_vector.z = sin(radians(yaw)) * cos(radians(pitch));
  // normalize the vectors, otherwise their length gets closer to 0 the more you look up or down which results in slower movement.
  forward_vector = normalize(forward_vector);
  right_vector = normalize(cross(forward_vector, world_up_vector));
  up_vector = normalize(cross(right_vector, forward_vector));
}

void Camera::updateViewToProj()
{
  view_to_projection = glm::perspective(glm::radians(fov_degrees), 1.25f, .1f, 100.0f);
}