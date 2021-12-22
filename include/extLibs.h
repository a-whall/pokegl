#pragma once
// This file is a remnant from when this project was header only,
// it is no longer included anywhere and some of these dependencies
// are probably no longer used.

// std cpp
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <bitset>

// glew / glm
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

// sdl
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_opengl.h"