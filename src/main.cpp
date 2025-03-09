

#include <raylib.h>
#include <rlgl.h> // NOTE: Make sure raylib is compiled with opengl 4.3

#include <stdlib.h> // For NULL
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
using namespace glm;
#define CPP
#include "object.h"
#include "march.h"

#include "state.hpp"
#include "utility.hpp"

#include <vector>

int main() {

	InitWindow(1280, 720, "Fractal Flyer");
	
	State::init();
	auto state = State::get_instance();

	auto & objects = state->objects;

	objects.push_back(Object {
		SDF_SPONGE,
		INTERSECTION_UNION,
		mat4(1.0)
	});
	objects.push_back(
	Object {
		SDF_SPHERE,
		INTERSECTION_SUBTRACT,
		mat4(1.0)
	});

	state->update_object_shader_buffer();

	float aspect_ratio = (float)GetRenderWidth() / (float)GetRenderHeight();

	auto march_shader = state->march_shader;

	set_uniform(state->march_shader, "aspect_ratio", &aspect_ratio, RL_SHADER_UNIFORM_FLOAT);

	state->player.set_position(vec3(0.0, 0.0, -1.0));

	while (!WindowShouldClose()) {

		rlBindShaderBuffer(state->shader_buffer, 1);
		set_uniform_matrix(state->march_shader, "eye", state->player.get_camera_matrix());
		state->update_object_shader_buffer();

		state->player.get_view_angles() += vec3(GetMouseDelta().y / 100.0, GetMouseDelta().x / 100.0, 0.0); 
		state->player.get_position() += state->player.get_forward() * (IsKeyDown(KEY_W) ? 1.0f : 0.0f) * GetFrameTime();

		BeginDrawing();

			BeginShaderMode(march_shader);
				DrawTexture(state->white_texture, 0, 0, WHITE);
			EndShaderMode();

		EndDrawing();

	}

	CloseWindow();

	return 0;
}