#include "sound.h"
#include "debug.h"
#include <sstream>

using namespace Debug;

void Sound_Manager::load_sfx()
{
  for (auto& sID : {bump_wall, door_in, door_out,ledge_hop})
  {
    Mix_Chunk * mc = Mix_LoadWAV(get_sfx_path(sID));
    if (mc != NULL)
      sfx_map[sID] = mc;
    else
      log_error_from(sound,Mix_GetError());
  }
}

void Sound_Manager::play_song(Track_ID tID)
{
  if (loaded_song.ptr != nullptr) {
    Mix_FreeMusic(loaded_song.ptr);
    loaded_song.ptr = nullptr;
  }
  if ( loaded_song.ptr = Mix_LoadMUS(get_audio_path(tID)) ) {
    Mix_FadeInMusic(loaded_song.ptr, -1, 60);
    log_from(sound,"\n\t ",white1,"-:- now playing -:- ",get_audio_path(tID)+sizeof("assets/audio/music")," -:-",reset,'\n');
  }
  else
    log_error_from(sound,Mix_GetError());
}

void Sound_Manager::play_sfx(SFX_ID_enum sID)
{
  Mix_PlayChannel(-1, sfx_map[sID], 0);
}



const char * get_audio_path(Track_ID tID)
{
  if (tID == null_track) log_error_from(sound," get_audio_path recieved null track");
  switch(tID) {
    case new_bark_town_mp3:     return "assets/audio/music/new_bark_town.mp3";
    case route_29_mp3:          return "assets/audio/music/route_29.mp3";
    case elms_lab_mp3:          return "assets/audio/music/elms_lab.mp3";
    case cherry_grove_city_mp3: return "assets/audio/music/cherry_grove_city.mp3";
    default: return "file due to invalid Track ID enum";
  }
}

const char * get_sfx_path(SFX_ID sID)
{
  switch(sID) {
    case door_in: return "assets/audio/sfx/door_in.wav";
    case door_out: return "assets/audio/sfx/door_out.wav";
    case bump_wall: return "assets/audio/sfx/bump_wall.wav";
    case ledge_hop: return "assets/audio/sfx/ledge_hop.wav";
    case max_sfx_id: return "file due to invalue sfx id enum, MAX SFX ID";
    default: return "file due to get sfx path error";
  }
}


// constant conversion from map ID to track ID.
Track_ID_enum get_area_track(Map_ID_enum mID)
{
  switch(mID) {
    case elms_lab:
        return elms_lab_mp3;

    case player_house_fl1:
    case player_house_fl2:
    case new_bark_house_1:
    case elms_house:
    case new_bark_town:
        return new_bark_town_mp3;

    case route_29:
    case route_30:
        return route_29_mp3;

    case cherry_grove_city:
    case cherry_grove_house_1:
    case cherry_grove_house_2:
    case cherry_grove_house_3:
        return cherry_grove_city_mp3;

    case cherry_grove_pc:
    case violet_pc:
    case azalea_pc:
    case goldenrod_pc:
    case ecruteak_pc:
    case olivine_pc:
    case cianwood_pc:
    case mahogany_pc:
    case blackthorn_pc:
        return pokemon_center_mp3;

    default:
        return null_track;
  }
}