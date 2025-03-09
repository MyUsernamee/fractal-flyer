#include "player.hpp"
#include "utility.hpp"
#include <glm/ext/matrix_transform.hpp>

glm::vec3 Player::get_position() {

	return this->position;

}

glm::vec3 Player::get_view_angles() {

	return this->view_angles;

}

void Player::set_position(glm::vec3 new_position) {

	this->position = new_position;

}

void Player::set_view_angles(glm::vec3 new_view_angles) {

	this->view_angles = new_view_angles;

}

glm::mat4 Player::get_camera_matrix() {

	return glm::inverse(glm::lookAtLH(this->position, this->position + from_euler(this->view_angles), UP));

}