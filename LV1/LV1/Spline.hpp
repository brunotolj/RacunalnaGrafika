#pragma once
#include <GLM/glm.hpp>
#include <string>
#include <vector>

class Spline
{
	std::vector<glm::vec3> points;

public:
	bool readFromFile(std::string fileName);
	unsigned numberOfSegments();
	glm::vec3 position(float t);
	glm::vec3 direction(float t);
	glm::vec3 normal(float t);
};