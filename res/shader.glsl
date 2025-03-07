#version 430

layout(std430, binding = 1) buffer test
{
    float test_buffer[];
};
in vec2 fragTexCoord;
out vec4 finalColor;


void main () {

	finalColor = vec4(fragTexCoord, test_buffer[0], 1.0);

}