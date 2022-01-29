#include "cam.h"
#include <gtc/type_ptr.hpp>
#include <iostream>



Camera::Camera()
{
  pos  = glm::vec3(0, 0, 5);
  view = glm::lookAt(pos, glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
  proj = glm::perspective(glm::radians(70.0f), 1.25f, .1f, 100.0f);
  //proj = glm::ortho(-4.5f, +4.5f, -4.5f, 4.5f);
}



// increment the camera position by the given xy offset
void Camera::move(vec2 xy)
{
  std::cout << "cam moving\n";
  pos += glm::vec3(xy, 0);
  view = glm::lookAt(pos, glm::normalize(pos - glm::vec3(0, 0, -1)), glm::vec3(0, 1, 0));
}



// x y position adjustment
void Camera::set_Position(vec2 pos_xy)
{
  pos = glm::vec3(pos_xy, pos.z);
  view = glm::lookAt(pos, glm::normalize(pos + glm::vec3(0, 0, -1)), glm::vec3(0, 1, 0));
}