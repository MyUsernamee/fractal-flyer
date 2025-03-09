#pragma once
#include "player.hpp"
#define CPP
#include "object.h"

#include <vector>
#include <raylib.h>

// Game state singleton
class State {

private:


public:
	static State* init();
	
	static State* instance;
	static State* get_instance();	
	
	void update_object_shader_buffer(); // Updates the shader object buffer on the gpu
	
	Texture white_texture;
	Player player;
	std::vector<Object> objects;
	Shader march_shader;
	unsigned int shader_buffer;

};