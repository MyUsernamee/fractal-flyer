#include "state.hpp"
#include "utility.hpp"
#include <rlgl.h>
#include <march.h>


State* State::instance = nullptr;

State* State::init() {

	auto new_state = new State {};
	State::instance = new_state;

	new_state->march_shader = LoadShader(NULL, "res/shader.glsl");
	new_state->shader_buffer = rlLoadShaderBuffer(MAX_OBJECTS * sizeof(Object), NULL, RL_DYNAMIC_COPY);

	Image whiteImage = GenImageColor(1280, 720, WHITE);
	new_state->white_texture = LoadTextureFromImage(whiteImage);
	UnloadImage(whiteImage);

	new_state->player.object_index = new_state->objects.size();
	new_state->objects.push_back(Object{
		SDF_SPHERE,
		INTERSECTION_UNION,
		mat4(0.008)
	});


	return new_state;

};

State* State::get_instance() {

	return State::instance;

}

void State::update_object_shader_buffer() {

	if (player.object_index != objects.size() - 1) {
		auto temp = objects[player.object_index];
		objects.erase(objects.begin() + player.object_index);
		objects.push_back(temp);
		player.object_index = objects.size() - 1;
	}

	rlUpdateShaderBuffer(shader_buffer, objects.data(), sizeof(Object) * objects.size(), 0);
	int size = objects.size();
	set_uniform(march_shader, "global_object_count", &size, RL_SHADER_UNIFORM_INT);
}