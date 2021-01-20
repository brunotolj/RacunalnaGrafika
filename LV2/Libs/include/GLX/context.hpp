#pragma once
#include <GLX/controller.hpp>
#include <GLX/cursor.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

namespace glx
{
	class Context
	{
		Context(Context const&) = delete;
		Context& operator=(Context const&) = delete;

		static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
		static void handleMouse(GLFWwindow* window, double posX, double posY);

		GLFWwindow* window;
		glm::ivec2 windowSize, bufferSize;
		bool keyState[1024];
		size_t keyPress[1024];
		Cursor pointer;

	public:
		Context(GLint width, GLint height, bool fullscreen = false);
		~Context();

		GLfloat aspectRatio() const;
		void clear();
		glm::vec2 cursorMovement();
		bool keyPressed(size_t key) const;
		bool newKeyPress(size_t key);
		void pollEvents();
		void setBackgroundColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
		bool shouldClose() const;
		void swapBuffers();
	};

	class CloseWindow : public Controller
	{
		CloseWindow(CloseWindow const&) = delete;
		CloseWindow& operator=(CloseWindow const&) = delete;

		GLFWwindow* window;
	public:
		CloseWindow(GLFWwindow* window);
		virtual ~CloseWindow();
		virtual void tick(double delta, size_t channel = 0);
		virtual void trigger(size_t channel = 0);
	};
}