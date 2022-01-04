#include "sound.h"
#include "debug.h"

void Sound_Manager::load_sfx()
{
  for (auto sID : {bump_wall, door_in, door_out,ledge_hop})
  {
    Mix_Chunk * mc = Mix_LoadWAV("assets/audio/sfx/bump_wall.wav");
    if (mc != NULL) sfx_map[bump_wall] = mc;
    else Debug::log_error_from(Debug::sound,Mix_GetError());
  }
}

void Sound_Manager::play_song(Map_ID_enum mID)
{
  if (loaded_song != nullptr) {
    Mix_FreeMusic(loaded_song);
    loaded_song = nullptr;
  }
  if ( loaded_song = Mix_LoadMUS(get_audio_path(mID)) ) {
    Mix_PlayMusic(loaded_song, -1);
    Debug::log_from(Debug::sound,"mixer now playing ",get_audio_path(mID));
  }
  else
    Debug::log_error_from(Debug::sound,Mix_GetError());
}

void Sound_Manager::play_sfx(SFX_ID_enum sID)
{
  Mix_PlayChannel(-1, sfx_map[sID], 0);
}



const char * get_audio_path(Map_ID_enum mID)
{
  switch(mID)
  {
    case new_bark_town: return "assets/audio/music/new_bark_town.mp3";
    case route_29:      return "assets/audio/music/route_29.mp3";
    default:            return " due to invalid Map ID enum";
  }
}