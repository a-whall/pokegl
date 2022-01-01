#pragma once
#include "player.h"
#include "map.h"
#include "application.h"
#include "debug.h"
#include "warps.h"
#include "world.h"

namespace Game
{
  extern Player* p_player;
  extern Map* p_active_map;

  class Instance
  : public Application
  {
    World_Graph * world;
  public:
    void on_init() override;
    void on_update(float t) override;
    void on_exit() override;
  };
}