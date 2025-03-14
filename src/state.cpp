#include "state.hpp"
#include "utility.hpp"
#include <rlgl.h>
#include <march.h>


Game* Game::instance = nullptr;

Game* Game::init() {

	auto new_state = new Game {};
	Game::instance = new_state;

	new_state->march_shader = LoadShader(NULL, "res/shader.glsl");
	new_state->shader_buffer = rlLoadShaderBuffer(MAX_OBJECTS * sizeof(Object), NULL, RL_DYNAMIC_COPY);

	Image whiteImage = GenImageColor(1280, 720, WHITE);
	new_state->white_texture = LoadTextureFromImage(whiteImage);
	UnloadImage(whiteImage);

	new_state->objects.push_back(
		Object {
			SDF_WEIRD,
			INTERSECTION_UNION,
			WARP_NONE,
			mat4(1.0)
		}
	);

	new_state->player.get_position() += vec3(0.1, 0.0, 0.0);

	new_state->player.object_index = new_state->objects.size();
	new_state->objects.push_back(Object{
		SDF_SPHERE,
		INTERSECTION_UNION,
		WARP_NONE,
		mat4(0.008)
	});
	new_state->update_object_shader_buffer();

	return new_state;

};

Game* Game::get_instance() {

	return Game::instance;

}

void Game::update_object_shader_buffer() {

	if (instance->player.object_index != instance->objects.size() - 1) {
		auto temp = instance->objects[instance->player.object_index];
		instance->objects.erase(instance->objects.begin() + instance->player.object_index);
		instance->objects.push_back(temp);
		instance->player.object_index = instance->objects.size() - 1;
	}

	rlUpdateShaderBuffer(instance->shader_buffer, instance->objects.data(), sizeof(Object) * instance->objects.size(), 0);
	int size = instance->objects.size();
	set_uniform(instance->march_shader, "global_object_count", &size, RL_SHADER_UNIFORM_INT);
}

bool Game::should_close() {
	return WindowShouldClose();
}

void Game::main_loop() {

	float aspect_ratio = (float)GetRenderWidth() / (float)GetRenderHeight();
	set_uniform(instance->march_shader, "aspect_ratio", &aspect_ratio, RL_SHADER_UNIFORM_FLOAT);

	instance->player.update();

	rlBindShaderBuffer(instance->shader_buffer, 1);
	set_uniform_matrix(instance->march_shader, "eye", instance->player.get_camera_matrix());
	instance->update_object_shader_buffer();

	BeginDrawing();

		BeginShaderMode(instance->march_shader);
			DrawTexture(instance->white_texture, 0, 0, WHITE);
		EndShaderMode();

	EndDrawing();

}