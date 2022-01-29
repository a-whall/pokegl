#pragma once
#include <vector>
#include <unordered_map>
#include "glm.hpp"
#include "debug.h"

void compile(const int shader_id, const char* shader_file);

void log(const int, GLenum);

class Shader
{
  std::unordered_map<const char*, int> uniform_location_map;
  std::unordered_map<const char*, int> nonexistent_uniform_list;
  int get_uniform_location(const char* name);

public:

  GLuint handle;

  Shader(GLuint);
  ~Shader();

  void log_program_resources();

  Shader& set(const char* name, int i);
  Shader& set(const char* name, bool b);
  Shader& set(const char* name, float f);
  Shader& set(const char* name, float x, float y, float z);
  Shader& set(const char* name, float x, float y, float z, float w);
  Shader& set(const char* name, glm::vec2);
  Shader& set(const char* name, glm::vec3 v);
  Shader& set(const char* name, glm::vec4 v);
  Shader& set(const char* name, const glm::mat2 m);
  Shader& set(const char* name, const glm::mat3& m);
  Shader& set(const char* name, const glm::mat4& matrix);
};