#include "player.hpp"
#include "utility.hpp"
#include <glm/ext/matrix_transform.hpp>
#include "state.hpp"
#include "march.h"

using namespace glm;

glm::vec3 &Player::get_position() {

	return this->position;

}

glm::vec3 &Player::get_velocity() {

	return this->velocity;

}

glm::vec3 &Player::get_view_angles() {

	return this->view_angles;

}

glm::vec3 Player::get_forward() {

	return from_euler(view_angles);

}
glm::vec3 Player::get_right() {

	return cross(UP, from_euler(view_angles));

}
void Player::set_position(glm::vec3 new_position) {

	this->position = new_position;

}

void Player::set_view_angles(glm::vec3 new_view_angles) {

	this->view_angles = new_view_angles;

}

glm::mat4 Player::get_camera_matrix() {
	vec3 normal = get_world_normal_d(get_position(), State::get_instance()->objects.data(), State::get_instance()->objects.size() - 1, 0.002f);
	normal = (isnan(normal.x + normal.y + normal.z) ? UP : normal);


	return glm::inverse(glm::lookAtLH(this->position - from_euler(this->view_angles) * 0.02f, this->position, normal));

}

void Player::update() {

	State::get_instance()->objects[object_index].model_matrix[3] = vec4(position, 1.0);

	vec3 normal = get_world_normal(get_position(), State::get_instance()->objects.data(), State::get_instance()->objects.size() - 1);

	get_view_angles() += vec3(GetMouseDelta().y / 100.0, GetMouseDelta().x / 100.0, 0.0); 
	get_position() += get_velocity() * GetFrameTime(); // * 

	get_velocity() += (isnan(normal.x + normal.y + normal.z) ? -UP : -normal)* (float)GetFrameTime() * 0.1f +
	get_forward() * ((float)IsKeyDown(KEY_W) - (float)IsKeyDown(KEY_S)) * GetFrameTime() +
	get_right() * ((float)IsKeyDown(KEY_D) - (float)IsKeyDown(KEY_A)) * GetFrameTime();

	float d = full_sdf(get_position(), State::get_instance()->objects.data(), State::get_instance()->objects.size() - 1) ;

	if (d < 0.008){

		get_velocity() += -dot(normal, get_velocity()) * normal;
		get_velocity() *= 0.94;
		get_position() += normal * (0.008f - d); 

	}

}