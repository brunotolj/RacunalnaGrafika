#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>

class Shader
{
	Shader(Shader const&) = delete;
	Shader& operator=(Shader const&) = delete;

	GLuint id;

	void addShader(const char* code, GLenum type);
	void updateMVP();

public:

	glm::mat4 model, projection, view;

	Shader();
	~Shader();

	void compile(const char* vertexName, const char* fragmentName);
	void setModelMatrix(glm::mat4 const& model);
	void setProjectionMatrix(glm::mat4 const& projection);
	void setViewMatrix(glm::mat4 const& view);
	void setLife(double life);
	void setFactor(float factor);
	void use();
};