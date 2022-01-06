# a pokegl Makefile

# makes binaries with the following names
APPBIN = pokegl
DEBUGBIN = debug

# must update these lists if you create a new file
DEP_FILES := animation.h application.h camera.h collision.h debug.h frameid.h game.h map_orientation.h map.h mapid.h player.h scene.h sfxid.h shader.h sound.h sprite.h text.h warps.h world.h
OBJ_FILES := animation.o application.o camera.o collision.o debug.o game.o map.o main.o player.o scene.o shader.o sound.o sprite.o text.o warps.o world.o
MAIN_OBJ_FILE := main.o

# compiler and language version
CC := g++
CPP_STD := c++17

# project directories
IDIR := include
ODIR := obj
SDIR := src

# adds folder names to file names to make a list with appropriate paths
DEPS = $(patsubst %,$(IDIR)/%,$(DEP_FILES))
OBJS = $(patsubst %,$(ODIR)/%,$(OBJ_FILES))
MOBJ = $(patsubst %,$(ODIR)/%,$(MAIN_OBJ_FILE))

# required libraries: SDL, SDL_Image, SDL_Mixer, GLEW, OpenGL (GLM is header only)
# linux lib binaries
LIB = /usr/lib/x86_64-linux-gnu
# windows libs
SDL2_LIB = C:/cpplibs/SDL2mingw/main/lib
SDL_IMG_LIB = C:/cpplibs/SDL2mingw/image/lib
SDL_MXR_LIB = C:/cpplibs/SDL2mingw/mixer/lib
GFX_LIB = C:/cpplibs/Graphics# directory with GLEW32.lib, GLEW32s.lib, OpenGL32.lib

COMPILER_FLAGS =-Wpedantic -Wall -Wextra

LINKER_FLAGS =-lSDL2 -lSDL2main -lSDL2_image -lSDL2_mixer

ifeq ($(OS), Windows_NT)
	detected_os := windows
endif
ifeq ($(shell uname), Linux)
	detected_os := linux
endif

ifndef detected_os
$(error failed to detect OS)
endif

ifeq ($(detected_os), linux)
# linux clean up options
	RM_APP_FLAGS = -f $(APPBIN)
	RM_DBG_FLAGS = -f $(DEBUGBIN)
# linux library options
    LIBRARY_PATHS =-L$(LIB)
    LINKER_FLAGS += -lGLEW -lOpenGL
# linux includes
    GLM_DIR = /usr/include/glm
    GLEW_DIR = /usr/include/glew
    SDL2_DIR = /usr/include/SDL2
    SDL_IMG_DIR = /usr/include/SDL2_image
    SDL_MXR_DIR = /usr/include/SDL2_mixer
endif
ifeq ($(detected_os), windows)
# windows clean up options (assumes an environment like MinGW for g++ so that rm is provided)
	RM_APP_FLAGS = -f $(APPBIN).exe
	RM_DBG_FLAGS = -f $(DEBUGBIN).exe
# windows library options
	LIBRARY_PATHS =-L$(SDL2_LIB) -L$(SDL_IMG_LIB) -L$(SDL_MXR_LIB) -L$(GFX_LIB)
	LINKER_FLAGS += -lGLEW32 -lOpenGL32
# windows includes
	GLM_DIR = C:/cpplibs/GLM 
	GLEW_DIR = C:/cpplibs/GLew/glew-2.1.0/include
	SDL2_DIR = C:/cpplibs/SDL2mingw/Main/include
	SDL_IMG_DIR = C:/cpplibs/SDL2mingw/Image/include
	SDL_MXR_DIR = C:/cpplibs/SDL2mingw/Mixer/include
endif

INCLUDES =-I$(IDIR) -I$(GLM_DIR) -I$(GLEW_DIR) -I$(SDL2_DIR) -I$(SDL_IMG_DIR) -I$(SDL_MXR_DIR)

$(APPBIN): $(OBJS) $(MOBJ)
	$(CC) $^ -std=$(CPP_STD) $(COMPILER_FLAGS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -O3 -o $@

# enable default debug info for GDB
$(DEBUGBIN): $(OBJS) $(MOBJ)
	$(CC) -g $^ -std=$(CPP_STD) $(COMPILER_FLAGS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $@

all: $(APPBIN) $(DEBUGBIN)

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o
	rm $(RM_APP_FLAGS)
	rm $(RM_DBG_FLAGS)

$(OBJS): $(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) -c -std=$(CPP_STD) $< $(CFLAGS) $(INCLUDES) -o $@

# Library links for windows
# https://www.libsdl.org/release/SDL2-devel-2.0.14-mingw.tar.gz
# https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.5-mingw.tar.gz
# https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-2.0.4-mingw.tar.gz
# https://sourceforge.net/projects/glew/files/latest/download
# https://github.com/g-truc/glm/releases/download/0.9.9.7/glm-0.9.9.7.zip

# Linux
# https://wiki.libsdl.org/Installation