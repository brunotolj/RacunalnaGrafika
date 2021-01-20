#pragma once
#include <GL/glew.h>
#include <string>

class Mesh
{
	Mesh(Mesh const&) = delete;
	Mesh& operator=(Mesh const&) = delete;

	GLuint VAO, VBO, IBO, indexCount;

public:
	Mesh();
	~Mesh();

	bool readFromFile(std::string fileName);
	void draw() const;
};