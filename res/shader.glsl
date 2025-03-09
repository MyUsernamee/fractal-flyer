#version 430

uniform float aspect_ratio;
uniform int global_object_count;
uniform mat4 eye;

#include <object.h>

layout(std430, binding = 1) buffer object_buffer
{
    Object global_objects[MAX_OBJECTS];
};
in vec2 fragTexCoord;
out vec4 finalColor;

#define MARCH_H_IMPL
#include <march.h>

void main () {

	vec3 ray_direction = normalize(vec3(fragTexCoord.x * 2.0 - 1.0, -fragTexCoord.y * 2.0 + 1.0, 1.0) * vec3(aspect_ratio, 1.0, 1.0));
	vec4 ray_start = eye * vec4(0.0, 0.0, 0.0, 1.0);
	vec4 ray_end = eye * (vec4(ray_direction * 10.0 - vec3(0.0, 0.0, 3.0), 1.0));

	MarchData data = march(vec3(ray_start), vec3(ray_end), global_objects, global_object_count);
	if (data.intersection) {
		vec3 normal = get_world_normal(data.position, global_objects, global_object_count);
		MarchData shadow_march = march(data.position + normal * EPSILON, vec3(1.0, 1.0, -1.0) * 100.0, global_objects, global_object_count);


		float light_product = dot(normal, normalize(vec3(1.0, 1.0, -1.0)));
		float shadow_amount = shadow_march.intersection ? 0.0f : 1.0f;
		finalColor = vec4(vec3(pow(data.steps, -0.5) * 0.25 * (1.0 - shadow_amount) + max(light_product, 0.0) * shadow_amount), 1.0);
		// finalColor = vec4(vec3(data.intersection ? 1.0 : 0.0), 1.0);
	}
	else {
		finalColor = vec4(0.0, 0.0, 0.0, 1.0);
	}

	// finalColor = vec4(vec3(eye[int(floor(fragTexCoord.x * 4.0))][int(floor(fragTexCoord.y * 4.0))]* 0.5 + 0.5), 1.0);

}