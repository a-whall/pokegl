# a pokegl Makefile

# must update these lists if you create a new file
DEP_FILES = animation.h application.h camera.h collision.h debug.h extlibs.h frameid.h game.h map.h mapid.h player.h scene.h shader.h sprite.h warps.h world.h
OBJ_FILES = animation.o application.o camera.o collision.o debug.o game.o map.o player.o scene.o shader.o sprite.o warps.o world.o
MAIN_OBJ_FILE = main.o

# compiler and language version
CC = g++
CPP_STANDARD = -std=c++17

# project directories
IDIR = ./include
ODIR = ./obj
SDIR = ./src

# adds folder names to file names to make a list with appropriate paths
DEPS = $(patsubst %,$(IDIR)/%,$(DEP_FILES))
OBJS = $(patsubst %,$(ODIR)/%,$(OBJ_FILES))
MOBJ = $(patsubst %,$(ODIR)/%,$(MAIN_OBJ_FILE))

# library include directories (input your own if paths differ)
GLM_DIR = /usr/include/glm
GLEW_DIR = /usr/include/glew
SDL2_DIR = /usr/include/SDL2
SDL_IMG_DIR = /usr/include/SDL2_image
SDL_MXR_DIR = /usr/include/SDL2_mixer

INCLUDES =-I$(IDIR) -I$(GLM_DIR) -I$(GLEW_DIR) -I$(SDL2_DIR) -I$(SDL_IMG_DIR) -I$(SDL_MXR_DIR)

# linux lib binaries
LIB = /usr/lib/x86_64-linux-gnu

# windows libs (input your own paths, then choose the right LIBRARY_PATHS symbol below labeled windows)
SDL2_LIB = C:\cpplibs\SDL2mingw\main\lib
SDL_IMG_LIB = C:\cpplibs\SDL2mingw\image\lib
SDL_MXR_LIB = C:\cpplibs\SDL2mingw\mixer\lib
GFX_LIB = C:\cpplibs\Graphics# directory with GLEW32.lib, GLEW32s.lib, OpenGL32.lib

# libraries: SDL, SDL_Image, SDL_Mixer, GLEW32, OpenGL32
# linux
LIBRARY_PATHS =-L$(LIB)

# windows
# LIBRARY_PATHS =-L$(SDL2_LIB) -L$(SDL_IMG_LIB) -L$(SDL_MXR_LIB) -L$(GFX_LIB)

COMPILER_FLAGS =-Wpedantic -Wall -Wextra

LINKER_FLAGS =-lSDL2 -lSDL2main -lSDL2_image -lSDL2_mixer -lGLEW -lOpenGL

APPBIN = poke
DEBUGBIN = debug

all: $(APPBIN) $(DEBUGBIN)

$(APPBIN): $(OBJS) $(MOBJ)
	$(CC) $^ $(CPP_STANDARD) $(COMPILER_FLAGS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -O3 -o $@

# enable default debug info for GDB
$(DEBUGBIN): $(OBJS) $(MOBJ)
	$(CC) -g $^ $(CPP_STANDARD) $(COMPILER_FLAGS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $@

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o
	rm poke

$(ODIR)/%.o: $(SDIR)/%.cpp $()
	$(CC) -c $< $(CPP_STANDARD) $(CFLAGS) $(INCLUDES) -o $@

# Library links for windows
# https://www.libsdl.org/release/SDL2-devel-2.0.14-mingw.tar.gz
# https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.5-mingw.tar.gz
# https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-2.0.4-mingw.tar.gz
# https://sourceforge.net/projects/glew/files/latest/download
# https://github.com/g-truc/glm/releases/download/0.9.9.7/glm-0.9.9.7.zip

# Linux
# https://wiki.libsdl.org/Installation