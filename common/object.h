

#define MAX_OBJECTS 64

struct Object {
	int sdf_type;
	int intersection_type;
	#ifdef CPP
	alignas(16) 
	#endif
	mat4 model_matrix;
};
