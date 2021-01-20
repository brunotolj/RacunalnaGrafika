#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>

namespace glx
{
	class Shader
	{
		Shader(Shader const&) = delete;
		Shader& operator=(Shader const&) = delete;

		GLuint id;
		glm::mat4 model, projection, view;

		void addShader(const char* code, GLenum type);
		void updateMVP();

	public:
		Shader();
		~Shader();

		void compile(const char* vertexName, const char* fragmentName);
		void setCameraPosition(glm::vec3 const& position);
		void setLightColor(glm::vec3 const& color);
		void setLightDirection(glm::vec3 const& direction);
		void setMaterialGlow(glm::vec3 const& glow);
		void setMaterialShine(GLfloat shine);
		void setModelMatrix(glm::mat4 const& model);
		void setProjectionMatrix(glm::mat4 const& projection);
		void setViewMatrix(glm::mat4 const& view);
		void use();
	};
}