#include "Shader.hpp"
#include <fstream>
#include <string>
#include <GLM/gtc/type_ptr.hpp>

#include <iostream>

char* readFromFile(std::string path) {
	using namespace std;
	fstream file(path, ios::in);
	string data;
	char line[1024];
	while (!file.eof())
	{
		file.getline(line, 1024);
		data.append(line).append("\n");
	}
	char* code = new char[data.size() + 1];
	strcpy_s(code, data.size() + 1, data.c_str());
	return code;
}

Shader::Shader() : id(0), mvpLocation(0), invLocation(0), colorLocation(0), model(1.0f), view(1.0f), projection(1.0f) {}

Shader::~Shader() { glDeleteProgram(id); }

void Shader::addShader(const char* name, GLenum type) {
	std::ifstream shaderFile(std::string("Shaders\\").append(name).append(".glsl"));
	if (!shaderFile.is_open())
		throw;
	std::string code;
	char line[1024];
	while (!shaderFile.eof()) {
		shaderFile.getline(line, 1024);
		code.append(line).append("\n");
	}
	GLuint shader = glCreateShader(type);
	GLchar const* shaderCode[1]{ code.c_str() };
	GLint codeLength[1]{ static_cast<GLint>(code.size()) };
	glShaderSource(shader, 1, shaderCode, codeLength);
	glCompileShader(shader);
	GLint result(0);
	GLchar eLog[1024]{};
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << eLog << "\n";
		throw;
	}
	glAttachShader(id, shader);
	glDeleteShader(shader);
	return;
}

void Shader::updateMVP() {
	glm::mat4 mvp = projection * view * model;
	glm::mat3 inverseModel = glm::transpose(glm::inverse(model));
	use();
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix3fv(invLocation, 1, GL_FALSE, glm::value_ptr(inverseModel));
}

void Shader::compile(const char* vertexName, const char* fragmentName) {
	id = glCreateProgram();
	if (!id)
		throw "Error creating shader program";
	addShader(vertexName, GL_VERTEX_SHADER);
	addShader(fragmentName, GL_FRAGMENT_SHADER);
	GLint result(0);
	GLchar eLog[1024]{};
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(id, sizeof(eLog), NULL, eLog);
		throw eLog;
	}
	glValidateProgram(id);
	glGetProgramiv(id, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(id, sizeof(eLog), NULL, eLog);
		throw eLog;
	}
	mvpLocation = glGetUniformLocation(id, "mvp");
	invLocation = glGetUniformLocation(id, "inverseModel");
	colorLocation = glGetUniformLocation(id, "color");
	intensityLocation = glGetUniformLocation(id, "lightIntensity");
	directionLocation = glGetUniformLocation(id, "lightDirection");
	updateMVP();
}

void Shader::setModelMatrix(glm::mat4 const& model) {
	if (this->model == model)
		return;
	this->model = model;
	updateMVP();
}

void Shader::setProjectionMatrix(glm::mat4 const& projection) {
	if (this->projection == projection)
		return;
	this->projection = projection;
	updateMVP();
}

void Shader::setViewMatrix(glm::mat4 const& view) {
	if (this->view == view)
		return;
	this->view = view;
	updateMVP();
}

void Shader::setObjectColor(glm::vec3 const& color) {
	use();
	glUniform3fv(colorLocation, 1, glm::value_ptr(color));
}

void Shader::setLightIntensity(glm::vec3 const& intensity) {
	use();
	glUniform3fv(intensityLocation, 1, glm::value_ptr(intensity));
}

void Shader::setLightDirection(glm::vec3 const& direction) {
	use();
	glUniform3fv(directionLocation, 1, glm::value_ptr(direction));
}

void Shader::use() { glUseProgram(id); }