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

	return vec3(get_camera_matrix() * vec4(0.0, 0.0, 1.0, 0.0));

}
glm::vec3 Player::get_right() {

	return vec3(get_camera_matrix() * vec4(1.0, 0.0, 0.0, 0.0));

}
void Player::set_position(glm::vec3 new_position) {

	this->position = new_position;

}

void Player::set_view_angles(glm::vec3 new_view_angles) {

	this->view_angles = new_view_angles;

}

glm::mat4 Player::get_camera_matrix() {
	normal = get_world_normal_d(get_position(), State::get_instance()->objects.data(), State::get_instance()->objects.size() - 1, 0.0001f);
	normal = (isnan(normal.x + normal.y + normal.z) ? UP : normal);

	auto rotation_axis = cross((orientation_matrix[1]), normal);
	float magnitude = length(rotation_axis);
	if (magnitude < 0.00001f) {
		rotation_axis = orientation_matrix[0];
		magnitude = 1;
	}
	float angle = acos(dot(normal, orientation_matrix[1]));
	rotation_axis = normalize(rotation_axis);

	mat4 rotation_matrix; 
	vec3 view_direction; 
	

	if (abs(angle) > 0.01 && !isnan(angle)) {

		// Construct rotation matrix
		rotation_matrix = mat3(rotate(mat4(1.0), angle * GetFrameTime(), rotation_axis)) * orientation_matrix;
		orientation_matrix = rotation_matrix;
		view_direction = mat3(rotation_matrix) * from_euler(view_angles);
		for (int i = 0; i < 3; ++i) {
			orientation_matrix[i] = normalize(orientation_matrix[i]);
		}
		

	}
	else {
		view_direction = orientation_matrix * from_euler(view_angles);
	}

	return inverse(lookAtLH(position - view_direction * 0.02f, position, orientation_matrix[1]));

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

		get_velocity() += -dot(normal, get_velocity()) * normal * 1.4f;
		get_velocity() *= 0.94;
		get_position() += normal * (0.008f - d); 

	}

}