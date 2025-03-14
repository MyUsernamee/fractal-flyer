

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
	
	Game::init();
	auto state = Game::get_instance();

	while (!Game::should_close()) {
		Game::main_loop();
	}

	CloseWindow();

	return 0;
}