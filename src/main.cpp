

#include <raylib.h>
#include <rlgl.h> // NOTE: Make sure raylib is compiled with opengl 4.3

#include <stdlib.h> // For NULL
#include <stdio.h>

#include <glm/glm.hpp>
using namespace glm;
#include "march.h"

int main() {
	
	InitWindow(1280, 720, "Fractal Flyer");

	printf("%d\n", rlGetVersion());

	float test_float = 0.5f;
	unsigned int test_buffer = rlLoadShaderBuffer(sizeof(float), &test_float, RL_DYNAMIC_COPY);


	Shader test_shader = LoadShader(NULL, "res/shader.glsl");

	Image whiteImage = GenImageColor(1280, 720, WHITE);
	Texture whiteTexture = LoadTextureFromImage(whiteImage);
	UnloadImage(whiteImage);

	while (!WindowShouldClose()) {

		rlBindShaderBuffer(test_buffer, 1);

		BeginDrawing();

			BeginShaderMode(test_shader);
				DrawTexture(whiteTexture, 0, 0, WHITE);
			EndShaderMode();

		EndDrawing();

	}

	CloseWindow();

}