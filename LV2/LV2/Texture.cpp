#define STB_IMAGE_IMPLEMENTATION
#include "Texture.hpp"

Texture::Texture() : id(0), width(0), height(0), depth(0), fileLocation() {}

Texture::Texture(const char* fileLocation) : id(0), width(0), height(0), depth(0), fileLocation(fileLocation) { load(); }

Texture::~Texture() { clear(); }

void Texture::load(bool pixelated) {
	unsigned char* data = stbi_load(fileLocation, &width, &height, &depth, 0);
	if (!data)
		throw;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (pixelated) ? GL_NEAREST : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (pixelated) ? GL_NEAREST : GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}

void Texture::use() { glBindTexture(GL_TEXTURE_2D, id); }

void Texture::clear() {
	glDeleteTextures(1, &id);
	id = 0;
}