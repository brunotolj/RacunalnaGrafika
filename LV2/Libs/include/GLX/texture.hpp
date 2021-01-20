#pragma once
#include <GL/glew.h>
#include <stb_image.h>

namespace glx
{
	class Texture
	{
		GLuint id;
		int width, height, depth;
		const char* fileLocation;

	public:
		Texture();
		Texture(const char* fileLocation);
		~Texture();

		void load();
		void use();
		void clear();
	};
}