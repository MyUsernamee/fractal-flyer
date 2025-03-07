#version 430

layout(std430, binding = 1) buffer test
{
    float test_buffer[];
};
in vec2 fragTexCoord;
out vec4 finalColor;

#include <march.h>

void main () {

	vec3 ray_direction = normalize(vec3(fragTexCoord.x * 2.0 - 1.0, fragTexCoord * 2.0 - 1.0, 1.0))

	

	finalColor = vec4(, 1.0);

}