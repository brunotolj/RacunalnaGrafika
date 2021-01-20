#version 330

in vec3 normal;

out vec4 color;

void main()
{
	float light = (9.0 + 2.0 * normal.x + 3.0 * normal.y + 6.0 * normal.z) / 7.0;
	color = vec4(0.0, 0.5 * light, light, 1.0);
}