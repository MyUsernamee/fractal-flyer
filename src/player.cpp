#include "player.hpp"
#include "utility.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext.hpp>
#include "state.hpp"
#include "march.h"

using namespace glm;

glm::vec3 &Player::get_position() {

	return this->position;

}

glm::vec3 &Player::get_velocity() {

	return this->velocity;

}


glm::vec3 Player::get_forward() {

	return vec3(get_player_matrix() * vec4(0.0, 0.0, 1.0, 0.0));

}
glm::vec3 Player::get_right() {

	return vec3(get_player_matrix() * vec4(1.0, 0.0, 0.0, 0.0));

}
glm::vec3 Player::get_up() {
	return vec3(get_player_matrix() * vec4(0.0, 1.0, 0.0, 0.0));
}

void Player::set_position(glm::vec3 new_position) {

	this->position = new_position;

}

glm::mat4 Player::get_player_matrix() {
	glm::mat4 new_mat = mat4(orientation_matrix);
	new_mat[3] = vec4(get_position() - orientation_matrix * vec3(0.0, 0.0, 0.0), 1.0);

	return new_mat;	

}
glm::mat4 Player::get_camera_matrix() {

	glm::mat4 new_mat = mat4(orientation_matrix);
	new_mat[3] = vec4(get_position() - orientation_matrix * vec3(0.0, -0.03, 0.1), 1.0);

	return (new_mat);

}

void Player::update() {

	Game::get_instance()->objects[object_index].model_matrix[3] = vec4(position, 1.0);

	vec3 normal = get_world_normal(get_position(), Game::instance->objects.data(), Game::get_instance()->objects.size() - 1);

	mat3 pitch = mat3(rotate(mat4(1.0), -GetMouseDelta().y / 100.0f, vec3(1.0, 0.0, 0.0)) );
	mat3 roll = mat3(rotate(mat4(1.0), -GetMouseDelta().x / 100.0f, vec3(0.0, 0.0, 1.0)) );
	SetMousePosition(GetRenderWidth() / 2, GetRenderHeight() / 2);

	orientation_matrix = orientation_matrix * pitch * roll;

	get_position() += get_velocity() * GetFrameTime(); // * 

	get_velocity() -= UP * GetFrameTime() * GRAVITY;

	// Apply flying force
	get_velocity() = get_forward() * length(get_velocity());

	do_collision(normal);
	
}

void Player::do_collision(glm::vec3 normal) {

	float d = full_sdf(get_position(), Game::get_instance()->objects.data(), Game::get_instance()->objects.size() - 1) ;

	if (d < PLAYER_SIZE){

		get_velocity() += -dot(normal, get_velocity()) * normal * 1.4f;
		get_velocity() *= 0.94;
		get_position() += normal * (PLAYER_SIZE - d); 

	}

}