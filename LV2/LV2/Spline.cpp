#include "Spline.hpp"
#include <fstream>
using namespace std;
using namespace glm;

bool Spline::readFromFile(string filename)
{
	ifstream file("Meshes\\" + filename + ".txt");
	if (!file.is_open())
		return false;
	float a, b, c;
	file.peek();
	while (!file.eof())
	{
		file >> a >> b >> c;
		points.push_back(vec3(a, b, c));
		file.ignore(1i64, '\n');
		file.peek();
	}
	return true;
}

unsigned Spline::numberOfSegments() { return (points.size() > 3) ? (points.size() - 3) : 0; }

vec3 Spline::position(float t)
{
	if (t < 0.0f)
		t = 0.0f;
	if (t >= numberOfSegments())
		t = numberOfSegments() - 0.001f;
	unsigned segment = (int)t;
	float offset = t - segment;
	static constexpr mat4 matrix(-1, 3, -3, 1, 3, -6, 0, 4, -3, 3, 3, 1, 1, 0, 0, 0);
	mat4x3 knots(points[segment], points[segment + 1], points[segment + 2], points[segment + 3]);
	vec4 tvec(offset * offset * offset, offset * offset, offset, 1.0f);
	return tvec * (matrix * transpose(knots)) / vec3(6);
}

vec3 Spline::direction(float t)
{
	if (t < 0.0f)
		t = 0.0f;
	if (t >= numberOfSegments())
		t = numberOfSegments() - 0.001f;
	unsigned segment = (int)t;
	float offset = t - segment;
	static constexpr mat4x3 matrix(-1, 2, -1, 3, -4, 0, -3, 2, 1, 1, 0, 0);
	mat4x3 knots(points[segment], points[segment + 1], points[segment + 2], points[segment + 3]);
	vec3 tvec(offset * offset, offset, 1.0f);
	return tvec * (matrix * transpose(knots)) / vec3(2);
}

vec3 Spline::normal(float t)
{
	if (t < 0.0f)
		t = 0.0f;
	if (t >= numberOfSegments())
		t = numberOfSegments() - 0.001f;
	unsigned segment = (int)t;
	float offset = t - segment;
	static constexpr mat4x2 matrix(-1, 1, 3, -2, -3, 1, 1, 0);
	mat4x3 knots(points[segment], points[segment + 1], points[segment + 2], points[segment + 3]);
	vec2 tvec(offset, 1.0f);
	return cross(direction(t), tvec * (matrix * transpose(knots)));
}