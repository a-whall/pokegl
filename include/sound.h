#include <SDL.h>
#include <SDL_mixer.h>
#include <unordered_map>
#include "mapid.h"
#include "soundid.h"

const char * get_audio_path(Track_ID);
const char * get_sfx_path(SFX_ID);
Track_ID get_area_track(Map_ID);

struct Song
{
  Track_ID tid = null_track;
  Mix_Music * ptr = nullptr;
};

class Sound_Manager
{
  Song loaded_song;
  std::unordered_map<SFX_ID_enum,Mix_Chunk*> sfx_map;
public:
  void load_sfx();
  void play_song(Track_ID);
  void play_sfx(SFX_ID_enum);
};