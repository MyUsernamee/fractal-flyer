#pragma once
#include "player.hpp"
#include "object.h"

// Game state singleton
struct State {

	static State* init();

	static State* instance;
	static State* get_instance();	

	Player player;
	std::vector<Object> objects;

};