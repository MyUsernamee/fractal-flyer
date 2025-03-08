

#include <raylib.h>
#include <rlgl.h> // NOTE: Make sure raylib is compiled with opengl 4.3

#include <stdlib.h> // For NULL
#include <stdio.h>

#include <glm/glm.hpp>
#include <cmath>
using namespace glm;
#include "object.h"
#include "march.h"

int main() {
	
	InitWindow(1280, 720, "Fractal Flyer");

	printf("%d\n", rlGetVersion());

	float test_float = 0.5f;
	unsigned int test_buffer = rlLoadShaderBuffer(sizeof(float), &test_float, RL_DYNAMIC_COPY);

	float aspect_ratio = (float)GetRenderWidth() / (float)GetRenderHeight();
	vec3 eye_position = vec3(0.0, 0.0, -1.2); 

	Shader test_shader = LoadShader(NULL, "res/shader.glsl");
	auto aspect_ratio_location = GetShaderLocation(test_shader, "aspect_ratio");
	auto eye_position_location = GetShaderLocation(test_shader, "eye_position");
	SetShaderValue(test_shader, aspect_ratio_location, &aspect_ratio, RL_SHADER_UNIFORM_FLOAT);
	SetShaderValue(test_shader, eye_position_location, &eye_position, RL_SHADER_UNIFORM_VEC3);

	Image whiteImage = GenImageColor(1280, 720, WHITE);
	Texture whiteTexture = LoadTextureFromImage(whiteImage);
	UnloadImage(whiteImage);

	while (!WindowShouldClose()) {

		eye_position = vec3(sin(GetTime() * 0.01), 0.0, -cos(GetTime() * 0.01)) * 2.0f;

		rlBindShaderBuffer(test_buffer, 1);
		SetShaderValue(test_shader, eye_position_location, &eye_position, RL_SHADER_UNIFORM_VEC3);

		BeginDrawing();

			BeginShaderMode(test_shader);
				DrawTexture(whiteTexture, 0, 0, WHITE);
			EndShaderMode();

		EndDrawing();

	}

	CloseWindow();

}