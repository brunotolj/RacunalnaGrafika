#pragma once
#include <GL/glew.h>

namespace glx
{
	class Mesh
	{
		Mesh(Mesh const&) = delete;
		Mesh& operator=(Mesh const&) = delete;

		GLuint VAO, VBO, IBO, indexCount;

	public:
		Mesh();
		~Mesh();

		void create(GLfloat* vertexData, GLsizeiptr vertexDataSize, GLuint* indexData, GLsizeiptr indexDataSize);
		void draw();
	};
}