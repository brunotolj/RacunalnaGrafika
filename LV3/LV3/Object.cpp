#include "Object.hpp"

Object::Object() : transform(1.0f), parent(nullptr), mesh(nullptr) {}

Object::Object(Object& parent) : transform(1.0f), parent(&parent), mesh(nullptr) {}

void Object::draw(Shader& shader) const {
	if (mesh) {
		shader.setModelMatrix(getGlobalTransform());
		mesh->draw();
	}
}

void Object::update(double deltaTime) {}

glm::mat4 Object::getGlobalTransform() const {
	if (parent)
		return parent->getGlobalTransform() * transform;
	return transform;
}

glm::mat4 Object::getLocalTransform() const { return transform; }

void Object::setLocalTransform(glm::mat4 transform) { this->transform = transform; }

void Object::setMesh(Mesh const& mesh) { this->mesh = &mesh; }

void Object::removeMesh() { mesh = nullptr; }
