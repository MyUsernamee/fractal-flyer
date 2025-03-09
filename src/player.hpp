#pragma once

#include <glm/glm.hpp>

class Player {

private:
	glm::vec3 position;
	glm::vec3 view_angles;

public:

	glm::vec3 get_position();
	glm::vec3 get_view_angles();

	glm::mat4 get_camera_matrix();

};