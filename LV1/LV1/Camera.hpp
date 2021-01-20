#pragma once
#include "Object.hpp"

class Camera : public Object
{
	float fov;
	glm::vec2 depth;

public:
	Camera(float fov = 90.0f);

	glm::vec3 getPosition() const;
	glm::mat4 getProjectionMatrix(float aspectRatio) const;
	glm::mat4 getViewMatrix() const;
	void setDepth(glm::vec2 const& depth);
};