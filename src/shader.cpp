#pragma dfjsf
#include "shader.h"
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cstring>

Shader::Shader(GLuint gl_program_ID)
{
  Debug::log_from(Debug::shader,"initializing: ",std::hex,this,std::dec," (program) ",gl_program_ID);
  handle = gl_program_ID;
}

Shader::~Shader()
{
  Debug::log_from(Debug::shader,"deleting: ",std::hex,this,std::dec," (program) ",handle);
  glDeleteProgram(handle);
}

int Shader::get_uniform_location(const char *name)
{
  if (uniform_location_map.find(name) != uniform_location_map.end())
    return uniform_location_map[name];

  else if (nonexistent_uniform_list.find(name) != nonexistent_uniform_list.end())
    return -1;

  int location = glGetUniformLocation(handle, name);
  if (location == -1) {
    Debug::log_error_from(Debug::shader,"cache warning: '",name,"' is not an active uniform,",
    " it either doesn't exist or the OpenGL compiler optimized it out of program ",handle);
    nonexistent_uniform_list[name]= -1;
  }
  else {
    uniform_location_map[name]= location;
    Debug::log_from(Debug::shader,"program ",handle," uniform variable '",name,"' location has been cached");
  }
  return location;
}

Shader* Shader::set(const char* name, int i)                  { glUniform1i(get_uniform_location(name), i);                                 return this; }
Shader* Shader::set(const char* name, bool b)                 { glUniform1i(get_uniform_location(name), (int)b);                            return this; }
Shader* Shader::set(const char* name, float f)                { glUniform1f(get_uniform_location(name), f);                                 return this; }
Shader* Shader::set(const char* name, float x, float y, float z){ glUniform3f(get_uniform_location(name), x, y, z);                         return this; }
Shader* Shader::set(const char* name, float x, float y, float z, float w){ glUniform4f(get_uniform_location(name), x, y, z, w);             return this; }
Shader* Shader::set(const char* name, glm::vec3 v)            { glUniform3f(get_uniform_location(name), v.x, v.y, v.z);                     return this; }
Shader* Shader::set(const char* name, glm::vec4 v)            { glUniform4f(get_uniform_location(name), v.x, v.y, v.z, v.w);                return this; }
Shader* Shader::set(const char* name, const glm::mat3& m)     { glUniformMatrix3fv(get_uniform_location(name), 1, GL_FALSE, &m[0][0]);      return this; }
Shader* Shader::set(const char* name, const glm::mat4& matrix){ glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, &matrix[0][0]); return this; }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void compile(const int shader_id, const char* shader_file)
{
  using namespace std;
  using namespace Debug;

  // compiler state variables
  GLchar * error_log;
  string line;
  stringstream ss[6];
  GLuint handles[6] = {0,0,0,0,0,0};
  ifstream input_stream (shader_file);
  shader_t t = NONE;
  GLint status_errno, log_length, line_count = 0;

  // helper to determine if the current line contains a given phrase
  auto line_contains = [&line](string phrase)
  {
    return line.find(phrase) != string::npos;
  };

  // helper to convert an integer to a string representation of shader type
  auto str = [](GLint i)
  {
    switch(i) {
      case VERT: return"vertex";
      case TESC: return"tes control";
      case TESE: return"tes evaluate";
      case GEOM: return"geometry";
      case FRAG: return"fragment";
      case COMP: return"compute";
      default: return"<invalid shader type>";
    }
  };

  // PRE COMPILING ==================================================================================================================================
  log_from(compiler,"parsing shader source code from: ", shader_file);
  while (getline(input_stream, line))
  {
    // try to parse shader directive (Note: required to write to the correct string stream)
    if (line_contains("#shader")) {
      line_count= 0; // reset line count
      if      (line_contains("vertex"))       t=VERT;
      else if (line_contains("compute"))      t=COMP;
      else if (line_contains("geometry"))     t=GEOM;
      else if (line_contains("fragment"))     t=FRAG;
      else if (line_contains("tes control"))  t=TESC;
      else if (line_contains("tes evaluate")) t=TESE;
      else log_error_abort(compiler,"invalid shader preprocessing directive\n\t | #shader\n\t directive must be 1 of:\n\t 'vertex', 'compute', 'geometry', 'fragment', 'tes control', or 'tes evaluate'\n\t compilation terminated.\n");
    }
    // else in case the line contains an include directive: dump lines from a temporary input-file-stream
    else if (line_contains("#include")) {
      size_t start = line.find("<") + 1;
      size_t last  = line.find(">");
      if (start == -1 || last == -1)
        log_error_abort(compiler,"in ",shader_file,": #include is missing bracket(s) '<' or '>'");
      string specified_file (line.substr(start, last - start));
      string includePath ("shader/"+ specified_file +".glsl"); // Note expected path: shader/ *** .glsl
      ifstream temp_input(includePath);
      if (temp_input.fail())
        log_error_abort(compiler,specified_file,": No such file in shader directory\n\t ",shader_file,"\n\t | #include ",red,'<',specified_file,'>',reset,"\n\t compilation terminated.\n");
      while (getline(temp_input, line))
      {
        ss[t] << line << "\n";
        log_from(compiler,setw(2),line_count++,"| ",line); // log each line that gets input
      }
      continue;
    }
    // else in case the line contains an end directive: stop reading the glsl file
    else if (line_contains("#end")) {
      break;
    }
    // in any other case: the line gets pushed to the source code string stream for the shader of type t
    else {
      if (t == NONE)
        log_error_abort(compiler,"pokegl expects the first line of shader source code to be a #shader directive");
      ss[t] << line << "\n";
    }
    log_from(compiler,std::setw(2),line_count++,"| ",line);
  }
  // COMPILING ======================================================================================================================================
  for (GLuint i = VERT; i < 6; i++)
  {
    if (ss[i].tellp() != std::streampos(0))
    {
      handles[i] = glCreateShader(
        i == VERT ? GL_VERTEX_SHADER :
        i == FRAG ? GL_FRAGMENT_SHADER :
        i == TESC ? GL_TESS_CONTROL_SHADER :
        i == TESE ? GL_TESS_EVALUATION_SHADER :
        i == GEOM ? GL_GEOMETRY_SHADER :
        i == COMP ? GL_COMPUTE_SHADER :
        GL_INVALID_ENUM
      );

      if (handles[i] == 0)
        Debug::log_error_abort(Debug::compiler,"glCreateShader() failed");

      auto s = ss[i].str();
      const char* source_code = s.c_str();
      glShaderSource(handles[i], 1, &source_code, nullptr);
      glCompileShader(handles[i]);
      glGetShaderiv(handles[i], GL_COMPILE_STATUS, &status_errno);

      if (status_errno == GL_FALSE) {
        glGetShaderiv(handles[i], GL_INFO_LOG_LENGTH, &log_length);
        error_log = (GLchar*)malloc(log_length);
        glGetShaderInfoLog(handles[i], log_length, nullptr, error_log);
        error_log[strcspn(error_log,"\n")]= 0;
        Debug::log_error_abort(Debug::compiler,str(i)," shader compilation failed. In file ",shader_file,":", error_log);
      }
      else
        Debug::log_from(Debug::compiler,"compiled ",str(i)," unit successfully");

      glAttachShader(shader_id, handles[i]);
    }
  }
  // LINKING ========================================================================================================================================
  glLinkProgram(shader_id);
  glGetProgramiv(shader_id, GL_LINK_STATUS, &status_errno);
  // status_errno will be set to GL_FALSE if the last link operation was unsuccessful
  if (status_errno == GL_FALSE) {
    glGetProgramiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
    error_log = (GLchar*)malloc(log_length);
    glGetProgramInfoLog(shader_id, log_length, nullptr, error_log);
    error_log[strcspn(error_log,"\0") - 1] = 0;
    Debug::log_error_abort(Debug::compiler,"shader program failed to link.\n\n",error_log,'\n');
  }
  else {
    Debug::log_from(Debug::compiler,"linked program ",shader_id," successfully");
  }
  // CLEAN UP =======================================================================================================================================
  for (GLuint i = VERT; i < 6; i++)
  {
    if (handles[i] > 0)
    {
      glDetachShader(shader_id, handles[i]);
      glDeleteShader(handles[i]);
    }
  }
  // DONE ===========================================================================================================================================
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char* str(GLenum);
const char* type(GLenum);
void log(const int shader_id, GLenum requested_property)
{
  using std::setw; using std::left; using std::right; using std::vector;
  using namespace Debug;

  // this lambda querries OpenGL shader objects for info
  // @param properties:
  auto print_active = [&](vector<GLenum> properties)
  {
    int num_tokens;
    glGetProgramInterfaceiv(shader_id, requested_property, GL_ACTIVE_RESOURCES, &num_tokens);
    log_from(shader,"active ",str(requested_property)," of shader ",shader_id);
    log_from(shader,setw(5),"<index>",setw(9),"<name>",setw(40),"<type>");
    for(int i = 0; i < num_tokens; ++i)
    {
      int * results = new int[properties.size()];
      glGetProgramResourceiv(shader_id, requested_property, i, properties.size(), properties.data(), properties.size(), NULL, results);
      int size = results[0] + 1;
      char * name = new char[size];
      glGetProgramResourceName(shader_id, requested_property, i, size, NULL, name);
      log_from(shader,setw(4),results[2],"       ",left,setw(40),name,type(results[1]),right);
      delete[] results;
      delete[] name;
    }
  };
  // use the lambda above to get the following list of data
  switch(requested_property) {
    case GL_PROGRAM_INPUT: print_active({GL_NAME_LENGTH,GL_TYPE,GL_LOCATION}); break;
    case GL_UNIFORM: print_active({GL_NAME_LENGTH,GL_TYPE,GL_LOCATION,GL_BLOCK_INDEX}); break;
  }
}

const char* str(GLenum gl_program_interface_enum)
{
  using namespace Debug;
  switch(gl_program_interface_enum) {
    case GL_PROGRAM_INPUT: return"vertex attributes";
    case GL_UNIFORM:       return"uniforms";
    default: log_error_from(shader,"in function str(GLenum): unhandled GLenum value");
    return"<unhandled GLenum value>";
  }
}

const char* type(GLenum t)
{
  switch (t) {
    case GL_FLOAT:      return "float";
    case GL_FLOAT_VEC2: return "vec2";
    case GL_FLOAT_VEC3: return "vec3";
    case GL_FLOAT_VEC4: return "vec4";
    case GL_DOUBLE:     return "double";
    case GL_INT:         return "int";
    case GL_UNSIGNED_INT: return "unsigned int";
    case GL_BOOL:        return "bool";
    case GL_FLOAT_MAT2: return "mat2";
    case GL_FLOAT_MAT3: return "mat3";
    case GL_FLOAT_MAT4: return "mat4";
    case GL_SAMPLER_2D: return "sampler 2D";
    case GL_SAMPLER_2D_ARRAY: return "sampler 2D array";
    default: Debug::log_error_from(Debug::shader,"in function type(GLenum): unhandled gl type enum");
    return "???";
  }
}