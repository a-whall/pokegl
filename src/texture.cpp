#include "texture.h"
using std::vector;

#include <SDL_image.h>
using ::IMG_Load;

#include <cstring>
using std::memcpy;

#include "mapid.h"
using ::Map_ID_enum;

#include "debug.h"
using namespace Debug;



// This function is basically so that I don't have to repeat myself. 
// It calls glGenTextures for the caller, sets the active texture unit to 0, 
// binds the texture object to the desired binding target (which declares 
// to OpenGL what type of texture the object actually is). 
// This function assumes that you want to simply use texture unit 0 and nothing else. 
// This function also sets the texture object parameters such that no magnification or minification interpolation takes place. 
// This function binds the object to the given binding target, it is the callers responsibility to unbind the texture.
void generate_texture(GLenum target, GLuint * handle)
{
  glGenTextures(1, handle);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(target, *handle);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}



// This is a function that will load a list of given texture files using the SDL image function IMG_Load. 
// It loads the pixel data into a dynamically allocated array and returns a pointer to that array. 
// Caller must give a list of textures that : A ) all exist according to their path, 
// and B ) are of equal dimmensions with respect to width and height. 
// The caller is responsible for calling delete on the pointer to pixel data that is returned from this function. 
// Generally if a texture is not found, it's an easy fix 
// either don't send that bad path to this function or make it a good path, 
// so if it gets a bad path the program will crash and give a very informative message so that the bug can be fixed quickly.
// @param path_list: a reference to a std vector filled with c strings to be passed to IMG_Load in sequence
// @param s: the pokegl class that is using this function, TODO: test if this can work with Map_ID::none
// @param sender: the address of the pokegl class instance that is calling this function (only used for printing on an error)
// @param set_width,set_height: use these parameters to assign width and height somewhere else. (only samples the first texture loaded)
// @param BPP: Bytes Per Pixel
GLubyte *load_textures(Path_List path_list, Debug_Source_enum s, void *sender, int *set_width, int *set_height, GLsizei BPP)
{
  SDL_Surface *loaded_image= nullptr;
  GLubyte *p_pixel_data= nullptr;
  GLsizei BPF, offset= 0;
  bool first= true;

  for (auto& path : path_list)
  {
    loaded_image= IMG_Load(path);

    if (loaded_image == nullptr)
      log_error_abort(s,obj_addr(sender),": texture-load failed\n\t ",
        red,path,reset,"\n\t ",IMG_GetError(),'\n');

    if (first) {
      first=false;
      if (set_width != nullptr) *set_width= loaded_image->w;
      if (set_height != nullptr) *set_height= loaded_image->h;
      BPF = BPP * loaded_image->w * loaded_image->h;
      p_pixel_data= new GLubyte[BPF * path_list.size()];
    }

    memcpy(p_pixel_data + offset, loaded_image->pixels, BPF);
    SDL_FreeSurface(loaded_image);
    offset += BPF;
  }
  return p_pixel_data;
}