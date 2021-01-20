#version 330		

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 nor;

out vec3 normal;

uniform mat4 mvp;
uniform mat3 inverseModel;

void main()
{
	gl_Position = mvp * vec4(pos, 1.0);
	normal = normalize(inverseModel * nor);
}