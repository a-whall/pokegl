#include <SDL.h>
#include <SDL_mixer.h>
#include <unordered_map>
#include "mapid.h"
#include "sfxid.h"

const char * get_audio_path(Map_ID_enum);

class Sound_Manager
{
  Mix_Music * loaded_song = nullptr;
  std::unordered_map<SFX_ID_enum,Mix_Chunk*> sfx_map;
public:
  void load_sfx();
  void play_song(Map_ID_enum);
  void play_sfx(SFX_ID_enum);
};