#version 330		

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec4 data;

out vec2 TexCoords;
out float life;

uniform mat4 projection;

void main()
{
	TexCoords = tex;
	life = data.w;
	gl_Position = projection * vec4(vec3(pos, 0.0) + data.xyz, 1.0);
}