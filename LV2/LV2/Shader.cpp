#include "Shader.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <GLM/gtc/type_ptr.hpp>
using namespace std;

char* readFromFile(string path) {
	using namespace std;
	fstream file(path, ios::in);
	string data;
	char line[1024];
	while (!file.eof()) {
		file.getline(line, 1024);
		data.append(line).append("\n");
	}
	char* code = new char[data.size() + 1];
	strcpy_s(code, data.size() + 1, data.c_str());
	return code;
}

Shader::Shader() : id(0), model(1.0f), view(1.0f), projection(1.0f) {}

Shader::~Shader() { glDeleteProgram(id); }

void Shader::addShader(const char* name, GLenum type) {
	ifstream shaderFile(string("Shaders\\").append(name).append(".glsl"));
	string code;
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
		cout << eLog << endl;
		throw;
	}
	glAttachShader(id, shader);
	glDeleteShader(shader);
	return;
}

void Shader::updateMVP() {
	//GLint offsetLocation = glGetUniformLocation(id, "offset");
	glm::vec4 offset = view * model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	use();
	//glUniform3f(offsetLocation, offset[0], offset[1], offset[2]);
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
	GLint projectionLocation = glGetUniformLocation(id, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}

void Shader::setViewMatrix(glm::mat4 const& view) {
	if (this->view == view)
		return;
	this->view = view;
	updateMVP();
}

void Shader::setLife(double life) {
	GLint lifeLocation = glGetUniformLocation(id, "life");
	glUniform1f(lifeLocation, float(life));
}

void Shader::setFactor(float factor) {
	GLint factorLocation = glGetUniformLocation(id, "factor");
	glUniform1f(factorLocation, factor);
}

void Shader::use() { glUseProgram(id); }