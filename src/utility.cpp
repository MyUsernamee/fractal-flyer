
#include "utility.hpp"

Matrix convert_to_rlgl_matrix(glm::mat4 m) {
	return Matrix {
			m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]
		};	
	
}


glm::vec3 from_euler(glm::vec3 a) {
	return glm::vec3(sin(a.y) * cos(a.x), sin(a.x), cos(a.y) * cos(a.x));
} 

void set_uniform(Shader shader, const char* name, void* value, int type){
	int location = GetShaderLocation(shader, name);
	SetShaderValue(shader, location, value, type);
}
void set_uniform_matrix(Shader shader, const char* name, glm::mat4 value){
	unsigned int location = GetShaderLocation(shader, name);
	SetShaderValueMatrix(shader, location, convert_to_rlgl_matrix(value));
}