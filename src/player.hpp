#pragma once

#include <glm/glm.hpp>

class Player {

private:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 view_angles;

public:

	int object_index = 0;

	glm::vec3 &get_position();
	glm::vec3 &get_velocity();
	glm::vec3 &get_view_angles();
	glm::vec3 get_forward();
	glm::vec3 get_right();

	void set_position(glm::vec3 a);
	void set_view_angles(glm::vec3 a);

	glm::mat4 get_camera_matrix();

	void update();

};