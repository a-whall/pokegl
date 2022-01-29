#pragma once
#include <unordered_map>
#include <vector>
#include "mapid.h"
#include "texid.h"
#include "debug.h"
#include "pair.h"

// OpenGL 1.1 functions are not macros in glew.h 
// and many of the texture functions are that old. 
// For PokeGL, I would like macros to denote GL API interaction 
// and so for no reason other than visual consistency 
// with the modern GL API, I have defined these macros here.
#define glGenTextures glGenTextures
#define glBindTexture glBindTexture
#define glTexParameteri glTexParameteri
#define glTexImage2D glTexImage2D
#define glDeleteTextures glDeleteTextures

typedef unsigned GLenum, GLuint;
typedef int GLsizei;


typedef std::vector<const char *> Path_List;



void generate_texture(GLenum, GLuint*);



GLubyte * load_textures(Path_List path_list, Debug::Debug_Source_enum src, void *sender, int *set_width, int *set_height, GLsizei BPP);



Tex_ID get_map_tex(Map_ID);



namespace Texture_Data
{
  extern std::unordered_map<Tex_ID,Dim2D> bounds;
  Dim2D get_bounds(Tex_ID);
  Dim2D get_bounds(Map_ID);
  void def_bounds(Tex_ID, int, int);
  void def_bounds(Map_ID, int, int);
};