#include "Mesh.hpp"
#include "objLoader.hpp"
#include <fstream>
using namespace std;

Mesh::Mesh() : indexCount(0) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(1, &IBO);
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(2, VBO);
}

bool Mesh::readFromFile(string fileName) {
	ifstream file("Meshes\\" + fileName + ".imd", ios_base::in | ios_base::binary);
	if (!file.is_open()) {
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
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
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

void Mesh::createParticle() {
	float SIZE = 0.05f;
	GLfloat vertices[]{ -SIZE,-SIZE,0.0f,0.0f, -SIZE,SIZE,0.0f,0.125f, SIZE,SIZE,0.125f,0.125f, SIZE,-SIZE,0.125f,0.0f };
	GLuint indices[]{ 0,1,2,2,3,0 };

	GLuint vertexCount = 16;
	indexCount = 6;

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexCount, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (void*)(sizeof(GLfloat) * 2));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 0);
	glVertexAttribDivisor(2, 1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexCount, indices, GL_STATIC_DRAW);
}

void Mesh::draw(std::vector<Particle> const& particles, glm::mat4 transform) {
	GLfloat* data = new GLfloat[4 * particles.size()];
	for (GLuint i = 0; i < particles.size(); i++) {
		glm::vec4 pos = transform * glm::vec4(particles[i].Position, 1.0f);
		data[4 * i] = pos.x;
		data[4 * i + 1] = pos.y;
		data[4 * i + 2] = pos.z;
		data[4 * i + 3] = float(particles[i].Life / particles[i].MaxLife);
	}
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * particles.size(), data, GL_STATIC_DRAW);
	glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0, particles.size());
	delete[] data;
}