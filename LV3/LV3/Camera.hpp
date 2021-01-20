#pragma once
#include "Object.hpp"

class Camera : public Object {

	float fov;
	glm::vec2 depth;

public:

	Camera();

	glm::vec3 getPosition() const;
	glm::mat4 getProjectionMatrix(float aspectRatio) const;
	glm::mat4 getViewMatrix() const;

	void setFOV(float fov);
	void setDepth(glm::vec2 const& depth);
};