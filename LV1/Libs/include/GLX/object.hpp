#pragma once
#include <GLX/mesh.hpp>
#include <GLX/texture.hpp>
#include <GLX/shader.hpp>
#include <vector>

namespace glx
{
	class Object
	{
		Object& operator=(Object const&) = delete;

		glm::mat4 transform;
		glm::vec3 glow;
		Mesh& mesh;
		Shader& shader;
		Texture& texture;
		Object* parent;
		std::vector<Object*> children;

	public:
		Object(Mesh& mesh, Shader& shader, Texture& texture);
		Object(Mesh& mesh, Shader& shader, Texture& texture, Object& parent);
		virtual ~Object();

		void draw();
		glm::mat4 getGlobalTransform();
		glm::mat4 const& getLocalTransform();
		void setGlow(glm::vec3 const& glow);
		void setTransform(glm::mat4 const& model);
		virtual void update(GLfloat deltaTime) {};
	};
}