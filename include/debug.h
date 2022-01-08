#pragma once
#include <iostream>
#include <GL/glew.h>

namespace Debug
{
  // log functions of this namespace use the following output streams:
  using std::ostream;
  using std::cout;
  using std::cerr;

  // strings to change terminal font color
  const std::string red("\033[0;31m");
  const std::string green("\033[1;32m");
  const std::string yellow("\033[1;33m");
  const std::string cyan("\033[0;36m");
  const std::string magenta("\033[0;35m");
  const std::string reset("\033[0m");

  // OpenGL callback implementation
  void submit_debug_callback();
  extern void GLAPIENTRY my_debug_callback(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar *, const void *);



  // flags to control general program output
  enum Debug_Source_enum : uint32_t
  {
    none        = 0x00000000,
    animation   = 0x00000001,
    application = 0x00000002,
    camera      = 0x00000004,
    collision   = 0x00000008,
    compiler    = 0x00000010,
    stats       = 0x00000020,
    game        = 0x00000040,
    map         = 0x00000080,
    player      = 0x00000100,
    scene       = 0x00000200,
    shader      = 0x00000400,
    world       = 0x00000800,
    sound       = 0x00001000,
    warp        = 0x00002000,
    text        = 0x00004000,
    textbox     = 0x00008000,
    sprite      = 0x00010000,
    code        = 0x00020000,
    //open      = 0x00040000,
    //open      = 0x00080000,
    //open      = 0x00100000,
    //open      = 0x00200000,
    //open      = 0x00400000,
    //open      = 0x00800000,
    //open      = 0x01000000,
    //open      = 0x02000000,
    //open      = 0x04000000,
    //open      = 0x08000000,
    //open      = 0x10000000,
    //open      = 0x20000000,
    //open      = 0x40000000,
    //open      = 0x80000000,
    object      = text | player | map | shader | world,
    all         = 0xffffffff
  };
  constexpr unsigned output_filter= compiler | shader | code | stats;
  const char* str(Debug_Source_enum);



  // Below are pokegl's log functions used throughout the program for debug output

  template<ostream& stream, typename ... Args>
  void log(Args&&... args)
  {
    (stream << ... << args) << '\n';
  }

  template<typename ... Args>
  void log_error_abort(Debug_Source_enum src, Args&&... args)
  {
    log< cerr >(str(src),red,"fatal error: ",reset, std::forward<Args>(args)... , "\n[",magenta," Exit ",reset,"] ",magenta,EXIT_FAILURE,reset);
    exit(EXIT_FAILURE);
  }

  template<typename ... Args>
  void log_error_from(Debug_Source_enum src, Args&&... args)
  {
    log< cerr >(str(src),"error: ", std::forward<Args>(args)...);
  }

  template<typename ... Args>
  void log_error(Args&&... args)
  {
    (cerr << ... << args) << '\n';
  }

  template<typename ... Args>
  void log_from(Debug_Source_enum src, Args&&... args)
  {
    if (src & output_filter) log<cout>(str(src), std::forward<Args>(args)...);
  }
}