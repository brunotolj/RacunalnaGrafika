#include "RenderContext.hpp"
#include <iostream>
using namespace std;

void initializeGLFW()
{
	if (!glfwInit())
	{
		glfwTerminate();
		std::cout << "GLFW initialization failed\n";
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void initializeGLEW()
{
	GLenum statusCode = glewInit();
	if (statusCode != GLEW_OK)
	{
		glfwTerminate();
		cout << glewGetErrorString(statusCode);
	}
}

void RenderContext::keyboardInput(GLFWwindow* window, int key, int code, int action, int mode)
{
	RenderContext* context = static_cast<RenderContext*>(glfwGetWindowUserPointer(window));
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_ESCAPE)
			context->close();
		context->keys[key] = true;
	}
	else if (action == GLFW_RELEASE)
		context->keys[key] = false;
}

void RenderContext::mouseMoveInput(GLFWwindow* window, double posX, double posY)
{
	RenderContext* context = static_cast<RenderContext*>(glfwGetWindowUserPointer(window));
	static glm::vec2 oldPos(0.0f, 0.0f), newPos;
	static bool init = false;
	newPos = glm::vec2(posX, posY);
	if (init)
		*(context->mouseTarget) += oldPos - newPos;
	else
		init = true;
	oldPos = newPos;
}

RenderContext::RenderContext(GLint width, GLint height, bool fullscreen) : windowSize(width, height), mouseTarget(nullptr)
{
	initializeGLFW();
	window = glfwCreateWindow(width, height, "OpenGL window", (fullscreen) ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	if (!window)
	{
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
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSwapInterval(1);
}

RenderContext::~RenderContext() { glfwTerminate(); }

GLfloat RenderContext::aspectRatio() const { return static_cast<GLfloat>(bufferSize.x) / bufferSize.y; }

bool RenderContext::isKeyPressed(unsigned key) { return keys[key]; }

void RenderContext::pollEvents() { glfwPollEvents(); }

void RenderContext::removeMouseTarget() { mouseTarget = nullptr; }

void RenderContext::swapBuffers()
{
	glfwSwapBuffers(window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderContext::setBackgroundColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) { glClearColor(r, g, b, a); }

void RenderContext::setMouseTarget(glm::vec2& target) { mouseTarget = &target; }

bool RenderContext::shouldClose() const { return glfwWindowShouldClose(window); }

void RenderContext::close() { glfwSetWindowShouldClose(window, true); }