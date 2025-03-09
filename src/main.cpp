

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
	
	std::vector<Object> objects;
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

	InitWindow(1280, 720, "Fractal Flyer");

	unsigned int test_buffer = rlLoadShaderBuffer(sizeof(Object) * objects.size(), objects.data(), RL_DYNAMIC_COPY);

	float aspect_ratio = (float)GetRenderWidth() / (float)GetRenderHeight();
	mat4 eye = inverse(lookAtLH(vec3(0.0, 0.0, -2.0), glm::vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0)));
	printf("Eye matrix:\n");
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%f ", eye[j][i]);
		}
		printf("\n");
	}
	int object_count = objects.size();

	Shader march_shader = LoadShader(NULL, "res/shader.glsl");

	auto aspect_ratio_location = GetShaderLocation(march_shader, "aspect_ratio");
	auto eye_location = GetShaderLocation(march_shader, "eye");
	auto global_object_count_location = GetShaderLocation(march_shader, "global_object_count");

	SetShaderValue(march_shader, aspect_ratio_location, &aspect_ratio, RL_SHADER_UNIFORM_FLOAT);
	SetShaderValueMatrix(march_shader, eye_location, convert_to_rlgl_matrix(eye));
	SetShaderValue(march_shader, global_object_count_location, &object_count, RL_SHADER_UNIFORM_INT);

	Image whiteImage = GenImageColor(1280, 720, WHITE);
	Texture whiteTexture = LoadTextureFromImage(whiteImage);
	UnloadImage(whiteImage);

	while (!WindowShouldClose()) {

		rlBindShaderBuffer(test_buffer, 1);

		eye = inverse(lookAtLH(vec3(0.0, 0.0, -2.0), glm::vec3(0.0, 1.0, 0.0), vec3(0.0, 1.0, 0.0)));
		SetShaderValueMatrix(march_shader, eye_location, convert_to_rlgl_matrix(eye));

		BeginDrawing();

			BeginShaderMode(march_shader);
				DrawTexture(whiteTexture, 0, 0, WHITE);
			EndShaderMode();

		EndDrawing();

	}

	CloseWindow();

	return 0;
}