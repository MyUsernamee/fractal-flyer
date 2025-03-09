#include "state.hpp"
#include <rlgl.h>


State* State::instance = nullptr;

State* State::init() {

	auto new_state = new State {};
	State::instance = new_state;

	new_state->shader_buffer = rlLoadShaderBuffer(0, NULL, RL_DYNAMIC_COPY);

	return new_state;

};

State* State::get_instance() {

	return State::instance;

}

void State::update_object_shader_buffer() {
	rlUpdateShaderBuffer(shader_buffer, objects.data(), sizeof(Object) * objects.size(), 0);
}