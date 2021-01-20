#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

namespace glx
{
	class Camera
	{
		Camera(Camera const&) = delete;
		Camera& operator=(Camera const&) = delete;

		glm::mat3 coordinateSystem; // CoordinateSystem = (right, up, front)
		glm::vec3 position, worldUp;
		glm::vec2 orientation;
		GLfloat fieldOfView, aspectRatio;

		void update();

	public:
		Camera(glm::vec3 position, glm::vec3 worldUp, glm::vec2 orientation, GLfloat fieldOfView, GLfloat aspectRatio);
		~Camera();

		void control(glm::vec3 movement, glm::vec2 rotation);
		glm::vec3 getPosition() const;
		glm::mat4 projectionMatrix() const;
		glm::mat4 viewMatrix() const;
	};
}