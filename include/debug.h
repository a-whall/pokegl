#pragma once
#include <iostream>
#include <GL/glew.h>
#include "colorstring.h"

namespace Debug
{
  // used frequently throughout the project
  enum Debug_Source_enum : uint32_t;


  // important to this namespace:
  using std::ostream;
  using std::string;
  using std::cout;
  using std::cerr;
  using std::forward;


  // Eventually the goal is to add these calls to every class constructor in pokegl which may be used for Application stats in the future.
  string obj_addr(void *);
  void obj_identify(Debug_Source_enum, const string &, void *, const string &);


  // OpenGL callback implementation
  void submit_debug_callback();
  extern void GLAPIENTRY callback(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar *, const void *);


  // flags to control general program output
  enum Debug_Source_enum : uint32_t
  {
    none        = 0x00000000,
    animation   = 0x00000001,
    application = 0x00000002,
    camera      = 0x00000004,
    collision   = 0x00000008,
    compiler    = 0x00000010, // prints the compile status of individual shader stage shaders as well as link status of the final executable.
    stats       = 0x00000020, // prints any stats the Application class may collect.
    game        = 0x00000040,
    map         = 0x00000080,
    player      = 0x00000100,
    scene       = 0x00000200,
    shader      = 0x00000400, // prints information from pokegl Shader objects such as location cache notifications.
    world       = 0x00000800,
    sound       = 0x00001000,
    warp        = 0x00002000,
    text        = 0x00004000,
    textbox     = 0x00008000,
    sprite      = 0x00010000,
    code        = 0x00020000, // prints glsl source code as it is parsed from file.
    object      = 0x00040000, // turn on yellow alloc and dealloc notifications for objects in pokegl. The object must be enabled as well.
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
    all         = 0xffffffff
  };
  constexpr unsigned output_filter= object | sound | map | player | shader | stats;
  const char* str(Debug_Source_enum);


  // general terminal output log function.
  // other functions defined below follow this pattern but with predefined output added around the variadic arguments
  // @param args: a comma separated list of anything to print to the template specified output stream.
  template<ostream& stream, typename ... Args>
  void log(Args&&... args)
  {
    (stream << ... << args) << '\n';
  }


  // abort the program, forward the error message to standard error output stream.
  // @param src: required output source, this is to help identify the source of the bug that is crashing the program.
  // @param args: a comma separated list of anything.
  template<typename ... Args>
  void log_error_abort(Debug_Source_enum src, Args&&... args)
  {
    log< cerr >(str(src),red1,"fatal error: ",reset,forward<Args>(args)... , "\n[",magenta," Exit ",reset,"] ",magenta,EXIT_FAILURE,reset);
    exit(EXIT_FAILURE);
  }


  // print a source tag followed by a red "error: " followed by the remaining variadic arguments.
  // @param src,args: same semantics as log_error_abort
  template<typename ... Args>
  void log_error_from(Debug_Source_enum src, Args&&... args)
  {
    log<cerr>(str(src),red1,"error: ",reset,forward<Args>(args)...);
  }


  // print a source tag followed by the remaining variadic arguments iff the output source bit is 1 in the output filter.
  template<typename ... Args>
  void log_from(Debug_Source_enum src, Args&&... args)
  {
    if (src & output_filter)
      log<cout>(str(src),forward<Args>(args)...);
  }


  // print a highlighted green SUCCESS followed by the remaining variadic arguments.
  template<typename ... Args>
  void log_success(Debug_Source_enum src, Args&&... args)
  {
    if (src & output_filter)
      log<cout>(str(src),greenb," SUCCESS ",reset,forward<Args>(args)...);
  }


  // print a highlighted red FAILURE followed by the remaining variadic arguments.
  template<typename ... Args>
  void log_failure(Debug_Source_enum src, Args&&... args)
  {
    log<cout>(str(src),redb," FAILURE ",reset,forward<Args>(args)...);
  }
}