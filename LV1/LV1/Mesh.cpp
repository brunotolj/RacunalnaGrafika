#include "Mesh.hpp"
#include "objLoader.hpp"
#include <fstream>
using namespace std;

Mesh::Mesh() : indexCount(0)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

bool Mesh::readFromFile(string fileName)
{
	ifstream file("Meshes\\" + fileName + ".imd", ios_base::in | ios_base::binary);
	if (!file.is_open())
	{
		createIMD(fileName);
		file.open("Meshes\\" + fileName + ".imd", ios_base::in | ios_base::binary);
		if (!file.is_open())
			return 0;
	}

	GLuint vertexCount;
	file.read((char*)&vertexCount, sizeof(vertexCount));
	GLfloat* vertexData = new GLfloat[vertexCount];
	file.read((char*)vertexData, (streamsize)sizeof(*vertexData) * vertexCount);
	file.read((char*)&indexCount, sizeof(indexCount));
	GLuint* indexData = new GLuint[indexCount];
	file.read((char*)indexData, (streamsize)sizeof(*indexData) * indexCount);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*vertexData) * vertexCount, vertexData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 5));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*indexData) * indexCount, indexData, GL_STATIC_DRAW);

	delete[] vertexData;
	delete[] indexData;

	return true;
}

void Mesh::draw() const
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}