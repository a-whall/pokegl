#include "debug.h"

void Debug::my_debug_callback(
  GLenum e_gl_debug_source,
  GLenum e_gl_debug_type,
  GLuint id,
  GLenum e_gl_debug_severity,
  GLsizei length,
  const GLchar * msg,
  const void * param
  )
{
  const char * source_str, * type_str, * sev_str;

  switch (e_gl_debug_source) {
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   source_str = "Window Sys";  break;
    case GL_DEBUG_SOURCE_API:             source_str = "OpenGL";      break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: source_str = "OGLCompiler"; break;
    case GL_DEBUG_SOURCE_OTHER:           source_str = "Other";       break;
    // cases below will never be generated by OpenGL, they're reserved for user defined error messages 
    case GL_DEBUG_SOURCE_THIRD_PARTY:     source_str = "Lib";         break;
    case GL_DEBUG_SOURCE_APPLICATION:     source_str = "App";         break;
    default: source_str = "<unknown_error_source>";
  }
  switch (e_gl_debug_type) {
    case GL_DEBUG_TYPE_ERROR:               type_str = "error: ";        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_str = "warning: ";      break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  type_str = "warning: ";      break;
    case GL_DEBUG_TYPE_PORTABILITY:         type_str = "warning: ";      break;
    case GL_DEBUG_TYPE_PERFORMANCE:         type_str = "warning: ";      break;
    case GL_DEBUG_TYPE_MARKER:              type_str = "warning: ";      break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          type_str = "warning: ";      break;
    case GL_DEBUG_TYPE_POP_GROUP:           type_str = "warning: ";      break;
    case GL_DEBUG_TYPE_OTHER:               type_str = "notification: "; break;
    default: type_str = "<unknown_msg_type>";
  }
  switch (e_gl_debug_severity) {
    case GL_DEBUG_SEVERITY_HIGH:             sev_str = "high";     break;
    case GL_DEBUG_SEVERITY_MEDIUM:           sev_str = "med";      break;
    case GL_DEBUG_SEVERITY_LOW:              sev_str = "low";      break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:     sev_str = "";         break;
    default: sev_str = "<unkown_error_severity_level>";
  }
  Debug::log_error('[', source_str, "] ", type_str, msg);
}

void Debug::submit_debug_callback() {
  // Assure error message is printed upon crash
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  // Supply debug format callback function
  glDebugMessageCallback(my_debug_callback, nullptr);
  // Config state for debug output
  glDebugMessageControl(
    GL_DONT_CARE,           // source of debug msgs to enable or disable
    GL_DONT_CARE,           // type of debug msgs to enable or disable
    GL_DEBUG_SEVERITY_HIGH, // severity of debug msgs to enable or disable
    0,                      // length of array ids
    NULL,                   // address of the array of msg ids to enable or disable
    GL_TRUE                 // bool for whether the msgs in the array should be enabled or disabled
  );
}

const char* Debug::str(Debug_Source_enum src_enum)
{
  switch(src_enum)
  {
    case animation:   return "[Animat] ";
    case application: return "[Applic] ";
    case camera:      return "[Camera] ";
    case collision:   return "[Collis] ";
    case compiler:    return "[Compil] ";
    case stats:       return "[Stats ] ";
    case game:        return "[ Game ] ";
    case map:         return "[ Map  ] ";
    case player:      return "[Player] ";
    case scene:       return "[Scene ] ";
    case sound:       return "[Sound ] ";
    case shader:      return "[Shader] ";
    case text:        return "[ Text ] ";
    case world:       return "[World ] ";
    default:          return "[      ] ";
  }
}