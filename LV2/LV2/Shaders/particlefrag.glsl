#version 330

in vec2 TexCoords;
in float life;

out vec4 color;

uniform float factor;
uniform sampler2D textureSampler;

void main()
{
	if (life <= 0.0)
	{
		color = vec4(0.0);
	}
	else
	{
		vec2 tex = TexCoords;
		int frame = int(64 * (1.0 - life));
		tex.x += frame % 8 * 0.125;
		tex.y += frame / 8 * 0.125;
		vec4 temp = texture(textureSampler, tex);
		color = vec4(temp.rgb * life * factor, temp.a);
	}
}