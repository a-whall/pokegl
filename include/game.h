#pragma once
#include "player.h"
#include "maps.h"
#include "application.h"
#include "debug.h"
#include "warps.h"
#include "world.h"

class PokeGL
: public Application
{
public:
  void on_init() override;
  void on_update(float) override;
  void on_exit() override;
};