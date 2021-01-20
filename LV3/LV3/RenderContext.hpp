#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include "InputManager.hpp"

class RenderContext {

	RenderContext(RenderContext const&) = delete;
	RenderContext& operator=(RenderContext const&) = delete;

	static void keyboardInput(GLFWwindow* window, int key, int code, int action, int mode);
	static void mouseMoveInput(GLFWwindow* window, double posX, double posY);

	GLFWwindow* window;
	glm::ivec2 windowSize, bufferSize;
	InputManager* inputManager;

public:

	RenderContext(GLint width, GLint height, bool fullscreen = false);
	~RenderContext();

	GLfloat aspectRatio() const;
	void pollEvents();
	void swapBuffers();
	void setBackgroundColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);
	bool shouldClose() const;
	void close();

	void setInputManager(InputManager& inputManager);
	void removeInputManager();
};