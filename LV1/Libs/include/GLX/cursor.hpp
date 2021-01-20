#pragma once
#include <GLM/glm.hpp>

namespace glx
{
	class Cursor
	{
		glm::dvec2 pos, lastPos;
		bool init;

	public:
		Cursor();
		Cursor(glm::dvec2 const& position);
		glm::dvec2 getMovement();
		glm::dvec2 getPosition();
		void move(glm::dvec2 const& movement);
		void setPosition(glm::dvec2 const& position);
	};
}