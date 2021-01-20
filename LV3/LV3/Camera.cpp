#include "Camera.hpp"
#include <GLM/gtc/matrix_transform.hpp>

Camera::Camera() : fov(90.0f), depth(0.01f, 100.0f) {}

glm::vec3 Camera::getPosition() const { return glm::vec3(transform[3][0], transform[3][1], transform[3][2]); }

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const { return glm::perspective(glm::radians(fov), aspectRatio, depth.x, depth.y); }

glm::mat4 Camera::getViewMatrix() const {
	glm::vec4 pos = transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 front = transform * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 up = transform * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
	return glm::lookAt(glm::vec3(pos), glm::vec3(front), glm::vec3(up));
}

void Camera::setFOV(float fov) { this->fov = fov; }

void Camera::setDepth(glm::vec2 const& depth) { this->depth = depth; }