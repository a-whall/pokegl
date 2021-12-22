#pragma once
#include "SDL.h"
#include "GL/glew.h"
#include "sprite.h"
#include "map.h"
#include "camera.h"
#include "shader.h"

namespace Scene
{
	using std::vector;
	class Manager
	{
		vector<Sprite*> sprites;
    vector<Map*>    maps;
    vector<Shader*> shaders;

	public:

    Camera *camera_controller;
    const Uint8* keystates;

    void init_camera(int, int);

		template <typename T, typename... TArgs>
		T& new_sprite(TArgs&&... mArgs)
    {
			T* o(new T(std::forward<TArgs>(mArgs)...)); // forward this functions args to its constructor
      sprites.emplace_back(std::move(o));
			return *o;
		}
    //Player& new_player();
    Map&    new_map(Map_ID_enum mID, Camera& c, Shader& s);
    Shader& new_shader(const char *);

		void update(float t);
		void render();
    void clean();
		void refresh();
	};
}