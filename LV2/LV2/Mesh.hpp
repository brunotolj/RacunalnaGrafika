#pragma once
#include "Particle.hpp"
#include <GL/glew.h>
#include <string>

class Mesh
{
	Mesh(Mesh const&) = delete;
	Mesh& operator=(Mesh const&) = delete;

	GLuint VAO, VBO[2], IBO, indexCount;

public:
	Mesh();
	~Mesh();

	bool readFromFile(std::string fileName);
	void createParticle();
	void draw(std::vector<Particle> const& particles, glm::mat4 transform);
};