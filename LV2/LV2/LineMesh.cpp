#include "LineMesh.hpp"
using namespace std;
using namespace glm;

LineMesh::LineMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

LineMesh::~LineMesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

bool LineMesh::loadFromSpline(Spline spline, unsigned samples)
{
	vertexCount = 3 * samples;
	GLfloat *vertexData = new GLfloat[vertexCount];

	for (unsigned i = 0; i < samples; i++)
	{
		vec3 vertex = spline.position(spline.numberOfSegments() * i / (float)(samples-1));
		vertexData[3 * i] = vertex.x;
		vertexData[3 * i + 1] = vertex.y;
		vertexData[3 * i + 2] = vertex.z;
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*vertexData) * vertexCount, vertexData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
	glEnableVertexAttribArray(0);

	delete[] vertexData;
	vertexCount /= 3;

	return true;
}

bool LineMesh::loadFromSplineTangents(Spline spline, unsigned samples, float size)
{
	vertexCount = 6 * samples;
	GLfloat* vertexData = new GLfloat[vertexCount];

	for (unsigned i = 0; i < samples; i++)
	{
		vec3 vertex = spline.position(spline.numberOfSegments() * i / (float)(samples - 1));
		vec3 tangent = spline.direction(spline.numberOfSegments() * i / (float)(samples - 1));
		vertexData[6 * i] = vertex.x;
		vertexData[6 * i + 1] = vertex.y;
		vertexData[6 * i + 2] = vertex.z;
		vertexData[6 * i + 3] = vertex.x + size * tangent.x;
		vertexData[6 * i + 4] = vertex.y + size * tangent.y;
		vertexData[6 * i + 5] = vertex.z + size * tangent.z;
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*vertexData) * vertexCount, vertexData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
	glEnableVertexAttribArray(0);

	delete[] vertexData;
	vertexCount /= 3;

	return true;
}

void LineMesh::draw(bool connected) const
{
	glBindVertexArray(VAO);
	if (connected)
		glDrawArrays(GL_LINE_STRIP, 0, vertexCount);
	else
		glDrawArrays(GL_LINES, 0, vertexCount);
}