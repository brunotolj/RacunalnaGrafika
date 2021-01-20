#include "Object.hpp"

Object::Object() : transform(1.0f), parent(nullptr), mesh(nullptr) {}

Object::Object(Object& parent) : transform(1.0f), parent(&parent), mesh(nullptr) {}

glm::mat4 Object::getTransform() const
{
	if (parent)
		return parent->getTransform() * transform;
	return transform;
}

void Object::removeMesh() { mesh = nullptr; }

void Object::setMesh(Mesh const& mesh) { this->mesh = &mesh; }

void Object::setTransform(glm::mat4 transform) { this->transform = transform; }