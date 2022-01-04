#pragma once
#include <iostream>
#include <GL/glew.h>

namespace Debug
{
  using std::cout;
  using std::cerr;

  enum Debug_Source_enum : uint16_t
  {
    animation   = 0x0001,
    application = 0x0002,
    camera      = 0x0004,
    collision   = 0x0008,
    compiler    = 0x0010,
    stats       = 0x0020,
    game        = 0x0040,
    map         = 0x0080,
    player      = 0x0100,
    scene       = 0x0200,
    shader      = 0x0400,
    world       = 0x0800,
    sound       = 0x1000,
    warp        = 0x2000,
    //open      = 0x4000,
    //open      = 0x8000,
    all         = 0xffff
  };

  // set to zero to turn off general program output, operator<OR> any source enum values to "turn on" that output source 
  // note: prior to compiling, you should make clean for changes to affect the whole program correctly.
  constexpr unsigned output_filter= all;//player | scene | map | sound | stats;

  extern void GLAPIENTRY my_debug_callback(
    GLenum gl_debug_source,
    GLenum gl_debug_type,
    GLuint gl_error_id,
		GLenum gl_debug_severity,
    GLsizei length,
    const GLchar *msg,
    const void *param
  );

  void submit_debug_callback();

  const char* str(Debug_Source_enum);

  template<typename ... Args>
  void log(Args&&... args)
  {
    (cout << ... << args) << '\n';
  }

  template<typename ... Args>
  void log_error_abort(Debug_Source_enum src_id, Args&&... args)
  {
    cerr << str(src_id) << "fatal error: ";
    (cerr << ... << args) << '\n'; // fold expr (C++17): Binary left fold
    cout << "[ Exit ] " << EXIT_FAILURE << '\n';
    exit(EXIT_FAILURE);
  }

  template<typename ... Args>
  void log_error_from(Debug_Source_enum src_id, Args&&... args)
  {
    cout << str(src_id) << "error: ";
    (cerr << ... << args) << '\n';
  }

  template<typename ... Args>
  void log_error(Args&&... args)
  {
    (cerr << ... << args) << '\n';
  }

  template<typename ... Args>
  void log_from(Debug_Source_enum src_enum, Args&&... args)
  {
    if (src_enum & output_filter)
    {
      cout << str(src_enum);
      log(std::forward<Args>(args)...);
    }
  }
}