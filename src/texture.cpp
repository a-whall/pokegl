#include "texture.h"
using std::vector;
#include <SDL_image.h>
using ::IMG_Load;
#include <cstring>
using std::memcpy;
#include "mapid.h"
using ::Map_ID_enum;
#include "texid.h"
using ::Tex_ID_enum;
#include "debug.h"
using namespace Debug;



// This function creates a mipmap-complete texture object, 
// it calls glGenTextures for the caller and 
// binds the texture object to the desired binding target. 
// - sets the texture object parameters magnification and minification to gl nearest. 
// - binds the object to the given binding target, it is the callers responsibility to unbind the texture.
void generate_texture(GLenum target, GLuint * handle)
{
  glGenTextures(1, handle);
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
GLubyte * load_textures(Path_List path_list, Debug_Source_enum s, void *sender, int *set_width, int *set_height, GLsizei BPP)
{
  SDL_Surface * loaded_image= nullptr;
  GLubyte * p_pixel_data= nullptr;
  GLsizei BPF, offset= 0;
  bool first= true;

  for (auto& path : path_list)
  {
    loaded_image= IMG_Load(path);

    if (loaded_image == nullptr)
      log_error_abort(s,obj_addr(sender),": texture-load failed\n\t ",
        red,path,reset,"\n\t ",SDL_GetError(),'\n');

    if (first) {
      first=false;
      if (set_width != nullptr) *set_width= loaded_image->w;
      if (set_height != nullptr) *set_height= loaded_image->h;
      BPF= BPP * loaded_image->w * loaded_image->h;
      p_pixel_data= new GLubyte[BPF * path_list.size()];
    }

    memcpy(p_pixel_data + offset, loaded_image->pixels, BPF);
    SDL_FreeSurface(loaded_image);
    offset += BPF;
  }
  return p_pixel_data;
}



// Map_ID -> Tex_ID conversion.
Tex_ID get_map_tex(Map_ID mID)
{
  switch(mID) {
    // New Bark Town
    case new_bark_town:        return new_bark_town_tex;
    case player_house_fl1:     return player_house_fl1_tex;
    case player_house_fl2:     return player_house_fl2_tex;
    case elms_lab:             return elms_lab_tex;
    case elms_house:           return elms_house_tex;
    case new_bark_house_1:     return house_1_tex;
    // Route 29
    case route_29:             return route_29_tex;
    // Cherry Grove City
    case cherry_grove_city:    return cherry_grove_city_tex;
    case cherry_grove_house_1:
    case cherry_grove_house_2:
    case cherry_grove_house_3: return house_1_tex;
    case cherry_grove_mart:    return pokemart_tex;
    case cherry_grove_pc:      return pokecenter_tex;
    // Route 30
    case route_30:             return route_30_tex;
    default: log_error_abort(texture, "invalid Tex_ID\n"
      "\t get_map_tex does not map: ",+mID,' ',map_str[mID]);
  }
}



// This function attempts to associate boundaries to the given map ID. 
// It will only modify the bounds hashmap if an element has not already been specified for mID. 
// That way map IDs will only be able to associate with one set of boundaries for the duration of the program. 
// If one has already been set the function simply returns. The intention here is to automatically register bounds based on map width and height when the map texture is loaded rather than hard coding that data.
// @param mID: used as a key in the global (to Collision_Data namespace) bounds hashmap.
// @param w_tiles,h_tiles: becomes the upper bounds for when collision data of mID is accessed.
void Texture_Data::def_bounds(Tex_ID tID, int w_tiles, int h_tiles)
{
  float w (w_tiles);
  float h (h_tiles);
  bounds.insert(std::make_pair(tID, Dim2D{w, h}));
}



// convert Map_ID to Tex_ID and call the actual def bounds function.
void Texture_Data::def_bounds(Map_ID mID, int w_tiles, int h_tiles)
{
  def_bounds(get_map_tex(mID), w_tiles, h_tiles);
}



// searches the bounds map with the given Tex_ID as the key. 
// if the bounds do not exist an error is printed. 
// overworld collision depends on this functionality.
// @return a {w,h} Pair of texture boundaries (w and h in units of 16x16-pixel tiles).
Point Texture_Data::get_bounds(Tex_ID tID)
{
  if (bounds.find(tID) == bounds.end())
    log_error_abort(texture,"invalid Tex_ID\n\t "
      "boundaries for ",tID,tex_str[tID]," undefined\n"); // TODO: implement stringify Tex_ID

  return bounds.at(tID); 
}



// convert Map_ID to Tex_ID and call the actual get bounds function.
Point Texture_Data::get_bounds(Map_ID mID)
{
  return get_bounds(get_map_tex(mID));
}



std::unordered_map<Tex_ID,Point> Texture_Data::bounds;