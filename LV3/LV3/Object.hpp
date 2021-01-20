#pragma once
#include <GLM/glm.hpp>
#include "Shader.hpp"
#include "Mesh.hpp"

class Object {

	Object* parent;
	Mesh const* mesh;

protected:

	glm::mat4 transform;

public:

	Object();
	Object(Object& parent);

	void draw(Shader& shader) const;
	virtual void update(double deltaTime);

	glm::mat4 getGlobalTransform() const;
	glm::mat4 getLocalTransform() const;
	void setLocalTransform(glm::mat4 transform);

	void setMesh(Mesh const& mesh);
	void removeMesh();
};