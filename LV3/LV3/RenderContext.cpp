#include "RenderContext.hpp"

#include <iostream>

void initializeGLFW() {
	if (!glfwInit()) {
		glfwTerminate();
		std::cout << "GLFW initialization failed\n";
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 16);
}

void initializeGLEW() {
	GLenum statusCode = glewInit();
	if (statusCode != GLEW_OK) {
		glfwTerminate();
		std::cout << glewGetErrorString(statusCode);
	}
}

void RenderContext::keyboardInput(GLFWwindow* window, int key, int code, int action, int mode) {
	RenderContext* context = static_cast<RenderContext*>(glfwGetWindowUserPointer(window));
	if (context->inputManager)
		switch (action) {

		case GLFW_PRESS:
			context->inputManager->keyPress(key);
			break;

		case GLFW_RELEASE:
			context->inputManager->keyRelease(key);
			break;

		case GLFW_REPEAT:
			context->inputManager->keyRepeat(key);
			break;
		}

	if (action == GLFW_PRESS)
		if (key == GLFW_KEY_ESCAPE)
			context->close();
}

void RenderContext::mouseMoveInput(GLFWwindow* window, double posX, double posY) {
	RenderContext* context = static_cast<RenderContext*>(glfwGetWindowUserPointer(window));
	static glm::vec2 oldPos(0.0f, 0.0f), newPos;
	static bool init = false;
	newPos = glm::vec2(posX, posY);
	oldPos = newPos;
}

RenderContext::RenderContext(GLint width, GLint height, bool fullscreen) : windowSize(width, height), inputManager(nullptr) {
	initializeGLFW();
	window = glfwCreateWindow(width, height, "OpenGL window", (fullscreen) ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		throw "GLFW window creation failed";
	}
	glfwGetFramebufferSize(window, &bufferSize.x, &bufferSize.y);
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, keyboardInput);
	glfwSetCursorPosCallback(window, mouseMoveInput);
	initializeGLEW();
	glViewport(0, 0, bufferSize.x, bufferSize.y);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSwapInterval(1);
}

RenderContext::~RenderContext() { glfwTerminate(); }

GLfloat RenderContext::aspectRatio() const { return static_cast<GLfloat>(bufferSize.x) / bufferSize.y; }

void RenderContext::pollEvents() { glfwPollEvents(); }

void RenderContext::swapBuffers() {
	glfwSwapBuffers(window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderContext::setBackgroundColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) { glClearColor(r, g, b, a); }

bool RenderContext::shouldClose() const { return glfwWindowShouldClose(window); }

void RenderContext::close() { glfwSetWindowShouldClose(window, true); }

void RenderContext::setInputManager(InputManager& inputManager) { this->inputManager = &inputManager; }

void RenderContext::removeInputManager() { inputManager = nullptr; }