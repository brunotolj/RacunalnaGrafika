#pragma once
#include <GLM/glm.hpp>
#include "Shader.hpp"
#include "Mesh.hpp"

class Object
{
	Object* parent;
	Mesh const* mesh;

protected:
	glm::mat4 transform;

public:
	Object();
	Object(Object& parent);

	glm::mat4 getTransform() const;
	void removeMesh();
	void setMesh(Mesh const& mesh);
	void setTransform(glm::mat4 transform);
	virtual void update(double deltaTime) {}
};