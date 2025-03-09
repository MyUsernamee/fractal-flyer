#pragma once
#include "player.hpp"

// Game state singleton
struct State {

	static State* instance;
	static State* get_instance();	

	Player player;

};