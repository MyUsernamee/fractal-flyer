

#define MAX_OBJECTS 8

struct Object;

#ifndef OBJECT_H
#define OBJECT_H

#ifdef CPP
#include <glm/glm.hpp>
using namespace glm;
#endif

struct Object {
	int sdf_type;
	int intersection_type;
	#ifdef CPP
	alignas(16) 
	#endif
	mat4 model_matrix;
};

#endif
