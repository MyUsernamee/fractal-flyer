#pragma once

#include <glm/glm.hpp>
#include <cmath>
#include <raylib.h>

#define UP glm::vec3(0.0, 1.0, 0.0)

Matrix convert_to_rlgl_matrix(glm::mat4 m);
glm::vec3 from_euler(glm::vec3 a) ;
void set_uniform(Shader shader, const char* name, void* value, int type);
void set_uniform_matrix(Shader shader, const char* name, glm::mat4 value);