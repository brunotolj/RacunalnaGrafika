#version 330		

layout (location = 0) in vec3 pos;

uniform mat4 mvp;
uniform mat3 inverseModel;

void main()
{
	gl_Position = mvp * vec4(pos, 1.0);
}