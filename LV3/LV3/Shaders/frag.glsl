#version 330

in vec3 normal;

out vec4 finalColor;

uniform vec3 color;
uniform vec3 lightIntensity;
uniform vec3 lightDirection;

void main()
{
	float diffuseFactor = -dot(normalize(lightDirection), normalize(normal));
	float specularFactor = 0.0;
	if (diffuseFactor > 0.0)
	{
		specularFactor = -normalize(reflect(lightDirection, normal)).z;
		if (specularFactor > 0.0)
		{
			specularFactor = pow(specularFactor, 2.0);
		}
		else
		{
			specularFactor = 0.0;
		}
	}
	else
	{
		diffuseFactor = 0.0;
	}
	float light = lightIntensity.x + diffuseFactor * lightIntensity.y + specularFactor * lightIntensity.z;
	finalColor = vec4(color * light, 1.0);
}