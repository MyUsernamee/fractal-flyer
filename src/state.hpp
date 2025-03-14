#pragma once
#include "player.hpp"
#define CPP
#include "object.h"

#include <vector>
#include <raylib.h>

// Game state singleton
class Game {

private:


public:
	static Game* init();
	
	static Game* instance;
	static Game* get_instance();	
	
	static void main_loop();
	static bool should_close();

	static void update_object_shader_buffer(); // Updates the shader object buffer on the gpu
	
	Texture white_texture;
	Player player;
	
	std::vector<Object> objects;
	Shader march_shader;
	unsigned int shader_buffer;

};