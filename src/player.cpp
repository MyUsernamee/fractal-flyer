#include "player.hpp"
#include "utility.hpp"
#include <glm/ext/matrix_transform.hpp>

glm::vec3 Player::get_position() {

	return this->position;

}

glm::vec3 Player::get_view_angles() {

	return this->view_angles;

}

glm::mat4 Player::get_camera_matrix() {

	return glm::inverse(glm::lookAtLH(this->position, this->position + from_euler(this->view_angles), UP));

}