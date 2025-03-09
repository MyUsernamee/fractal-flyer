

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
#define MARCH_H_IMPL
#include "march.h"
#undef MARCH_H_IMPL

#include "state.hpp"
#include "utility.hpp"

#include <vector>

// Function to project a 3D point into 2D, draw it, and connect it with a line
void RenderPointWithLine2D(const glm::vec3& position, Vector2 origin, float scale, Color color) {

    Vector2 projectedPos = {
        origin.x + (position.x * scale - position.z * scale),
        origin.y - scale * (position.y - position.x - position.z)
    };

    // Line thickness changes based on depth (closer = thicker)

    // Draw a line from origin to the projected point
    DrawLineEx(origin, projectedPos, 4.0, Fade(color, 0.7f));

    // Render a circle at the projected position
    // DrawCircleV(projectedPos, 5.0f * depthEffect * scale, color);
}

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

	state->player.set_position(vec3(0.0, 0.0, -1.4));

	while (!WindowShouldClose()) {

		rlBindShaderBuffer(state->shader_buffer, 1);
		set_uniform_matrix(state->march_shader, "eye", state->player.get_camera_matrix());
		state->update_object_shader_buffer();

		state->player.update();

		BeginDrawing();

			BeginShaderMode(march_shader);
				DrawTexture(state->white_texture, 0, 0, WHITE);
			EndShaderMode();

			RenderPointWithLine2D(vec3(1.0, 0.0, 0.0), Vector2{100, 100}, 40.0, RED);
			RenderPointWithLine2D(vec3(0.0, 1.0, 0.0), Vector2{100, 100}, 40.0, GREEN);
			RenderPointWithLine2D(vec3(0.0, 0.0, 1.0), Vector2{100, 100}, 40.0, BLUE);
			RenderPointWithLine2D(state->player.normal, Vector2{100, 100}, 40.0, YELLOW);
			RenderPointWithLine2D(state->player.orientation_matrix[1], Vector2{100, 100}, 40.0, PURPLE);
			RenderPointWithLine2D(cross(state->player.orientation_matrix[1], state->player.normal), Vector2{100, 100}, 40.0, ORANGE);

			float dotProduct = dot(state->player.normal, state->player.orientation_matrix[1]);
			float angle = acos(dotProduct);
			DrawText(TextFormat("Angle: %.2f", angle), 10, 10, 20, WHITE);
		EndDrawing();

	}

	CloseWindow();

	return 0;
}