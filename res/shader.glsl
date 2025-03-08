#version 430

uniform float aspect_ratio;
uniform vec3 eye_position;

#include <object.h>

layout(std430, binding = 1) buffer test
{
    float test_buffer[];
};
in vec2 fragTexCoord;
out vec4 finalColor;

#include <march.h>

void main () {

	vec3 ray_direction = normalize(vec3(fragTexCoord.x * 2.0 - 1.0, -fragTexCoord.y * 2.0 + 1.0, 1.0) * vec3(aspect_ratio, 1.0, 1.0));
	
	MarchData data = march(eye_position, eye_position + ray_direction * 10.0);
	if (data.intersection) {
		vec3 normal = get_normal(data.position);
		MarchData shadow_march = march(data.position + normal * EPSILON, vec3(1.0, 1.0, -1.0) * 100.0);


		float light_product = dot(normal, normalize(vec3(1.0, 1.0, -1.0)));
		float shadow_amount = shadow_march.intersection ? 0.0f : 1.0f;
		finalColor = vec4(vec3(pow(data.steps, -0.5) * 0.25 * (1.0 - shadow_amount) + max(light_product, 0.0) * shadow_amount), 1.0);
	}
	else {
		finalColor = vec4(0.0, 0.0, 0.0, 1.0);
	}

}