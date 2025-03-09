#pragma once

#include <glm/glm.hpp>
#include <cmath>

#define UP glm::vec3(0.0, 1.0, 0.0)

glm::vec3 from_euler(glm::vec3 a) {

	return glm::vec3(sin(a.y) * cos(a.x), sin(a.x), cos(a.y) * cos(a.x));

} 