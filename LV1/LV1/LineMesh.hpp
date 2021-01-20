#pragma once
#include <GL/glew.h>
#include "Spline.hpp"

class LineMesh
{
	LineMesh(LineMesh const&) = delete;
	LineMesh& operator=(LineMesh const&) = delete;

	GLuint VAO, VBO, vertexCount;

public:
	LineMesh();
	~LineMesh();

	bool loadFromSpline(Spline spline, unsigned samples);
	bool loadFromSplineTangents(Spline spline, unsigned samples, float size = 1.0f);
	void draw(bool connected = true) const;
};