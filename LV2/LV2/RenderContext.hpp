#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

class RenderContext
{
	RenderContext(RenderContext const&) = delete;
	RenderContext& operator=(RenderContext const&) = delete;

	static void keyboardInput(GLFWwindow* window, int key, int code, int action, int mode);
	static void mouseMoveInput(GLFWwindow* window, double posX, double posY);

	GLFWwindow* window;
	glm::ivec2 windowSize, bufferSize;
	glm::vec2* mouseTarget;

	bool keys[1024];

public:
	RenderContext(GLint width, GLint height, bool fullscreen = false);
	~RenderContext();

	GLfloat aspectRatio() const;
	bool isKeyPressed(unsigned key);
	void pollEvents();
	void removeMouseTarget();
	void swapBuffers();
	void setBackgroundColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);
	void setMouseTarget(glm::vec2& target);
	bool shouldClose() const;
	void close();
};