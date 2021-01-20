#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>

class Shader {

	Shader(Shader const&) = delete;
	Shader& operator=(Shader const&) = delete;

	GLuint id;
	GLint mvpLocation, invLocation, colorLocation, intensityLocation, directionLocation;
	glm::mat4 model, projection, view;

	void addShader(const char* code, GLenum type);
	void updateMVP();

public:

	Shader();
	~Shader();

	void compile(const char* vertexName, const char* fragmentName);
	void setModelMatrix(glm::mat4 const& model);
	void setProjectionMatrix(glm::mat4 const& projection);
	void setViewMatrix(glm::mat4 const& view);
	void setObjectColor(glm::vec3 const& color);
	void setLightIntensity(glm::vec3 const& intensity);
	void setLightDirection(glm::vec3 const& direction);
	void use();
};