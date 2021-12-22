#pragma once
#include <iostream>
#include <GL/glew.h>

namespace Debug
{
  using std::cout;
	using std::cerr;
	using std::endl;

  // switch to zero to turn off general program output
  constexpr unsigned debug_output_level = 0; // { 0: Print only errors and crash reports    1: Log all program output }

	extern void GLAPIENTRY myDebugCallback(
    GLenum e_gl_debug_source,
    GLenum e_gl_debug_type,
    GLuint id,
		GLenum e_gl_debug_severity,
    GLsizei length,
    const GLchar *msg,
    const void *param
  );

	void submitDebugCallbackFunction();

  template<typename ... Args>
  void log(Args&&... args)
  {
    if (debug_output_level > 0)
      (cout << ... << args) << '\n';
  }

	template<typename ... Args>
	void log_error_abort(Args&&... args)
  {
		(cerr << ... << args) << '\n'; // fold expr (C++17): Binary left fold (I op ... op A) becomes: ((((I op a1) op a2) op ...) op aN)
		exit(EXIT_FAILURE);
	}

  template<typename ... Args>
  void log_error(Args&&... args)
  {
    (cerr << ... << args) << '\n';
  }
}