

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

#include <vector>

Matrix convert_to_rlgl_matrix(mat4 m) {
	return Matrix {
			m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]
		};	
	
}

int main() {
	
	State::init();
	auto state = State::get_instance();

	auto objects = state->objects;

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

	InitWindow(1280, 720, "Fractal Flyer");


	float aspect_ratio = (float)GetRenderWidth() / (float)GetRenderHeight();
	int object_count = objects.size();


	state->march_shader = LoadShader(NULL, "res/shader.glsl");
	auto march_shader = state->march_shader;

	auto aspect_ratio_location = GetShaderLocation(march_shader, "aspect_ratio");
	auto eye_location = GetShaderLocation(march_shader, "eye");
	auto global_object_count_location = GetShaderLocation(march_shader, "global_object_count");

	SetShaderValue(march_shader, aspect_ratio_location, &aspect_ratio, RL_SHADER_UNIFORM_FLOAT);
	SetShaderValue(march_shader, global_object_count_location, &object_count, RL_SHADER_UNIFORM_INT);

	Image whiteImage = GenImageColor(1280, 720, WHITE);
	Texture whiteTexture = LoadTextureFromImage(whiteImage);
	UnloadImage(whiteImage);

	while (!WindowShouldClose()) {

		rlBindShaderBuffer(state->shader_buffer, 1);
		SetShaderValueMatrix(march_shader, eye_location, convert_to_rlgl_matrix(state->player.get_camera_matrix()));

		BeginDrawing();

			BeginShaderMode(march_shader);
				DrawTexture(whiteTexture, 0, 0, WHITE);
			EndShaderMode();

		EndDrawing();

	}

	CloseWindow();

	return 0;
}