

#define MAX_OBJECTS 8


#ifdef CPP
#include <glm/glm.hpp>
using namespace glm;
#pragma once
#endif

struct Object {
	int sdf_type;
	int intersection_type;
	int warp_type;
	#ifdef CPP
	alignas(16) 
	#endif
	mat4 model_matrix;
	mat4 warp_matrix 
	#ifdef CPP 
	= mat4(1.0)
	#endif
	;
};
