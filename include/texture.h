#pragma once
#include <gl/glew.h>
#include <vector>
#include "debug.h"

typedef std::vector<const char *> Path_List;

GLubyte * load_textures(Path_List path_list, Debug::Debug_Source_enum src, void *sender, int *set_width, int *set_height, GLsizei BPP);