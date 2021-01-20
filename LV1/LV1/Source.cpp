#include "RenderContext.hpp"
#include "LineMesh.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include <GLM/gtx/transform.hpp>
#include <fstream>
using namespace std;
using namespace glm;

unsigned WIDTH = 1920, HEIGHT = 1080;
float CAMERA_SPEED = 10.0f, ANIMATION_SPEED = 0.5f;
bool FULLSCREEN = true;

int main()
{
	ifstream config("cfg.txt");
	/*if (config.is_open())
	{
		config >> WIDTH;
		config >> HEIGHT;
		config >> FULLSCREEN;
		config >> CAMERA_SPEED;
		config >> ANIMATION_SPEED;
	}*/
	RenderContext window(WIDTH, HEIGHT, FULLSCREEN);
	window.setBackgroundColor(0.1f, 0.1f, 0.1f);

	vec2 cameraRotation(0.0f);
	window.setMouseTarget(cameraRotation);

	Spline spline;
	spline.readFromFile("helix");

	LineMesh line, tangents;
	line.loadFromSpline(spline, 120);
	tangents.loadFromSplineTangents(spline, 18, 0.5f);

	Mesh mesh;
	mesh.readFromFile("car");

	Camera camera;
	camera.setTransform(translate(vec3(5.0f, -20.0f, 30.0f)));

	Shader shader, lineShader, tangentShader;
	shader.compile("vert", "frag");
	shader.setProjectionMatrix(camera.getProjectionMatrix(window.aspectRatio()));
	lineShader.compile("linevert", "linefrag");
	lineShader.setProjectionMatrix(camera.getProjectionMatrix(window.aspectRatio()));
	tangentShader.compile("linevert", "tanfrag");
	tangentShader.setProjectionMatrix(camera.getProjectionMatrix(window.aspectRatio()));

	double lastTime = glfwGetTime(), currentTime, deltaTime, totalTime = 0;

	while (!window.shouldClose())
	{
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		totalTime += deltaTime * ANIMATION_SPEED;
		if (totalTime > spline.numberOfSegments())
			totalTime -= spline.numberOfSegments();

		window.pollEvents();

		if (cameraRotation.y < -89.0f)
			cameraRotation.y = -89.0f;
		if (cameraRotation.y > 89.0f)
			cameraRotation.y = 89.0f;

		camera.setTransform(translate(camera.getPosition()));
		camera.setTransform(camera.getTransform() * rotate(radians(cameraRotation.x), vec3(0.0f, 0.0f, 1.0f)));
		camera.setTransform(camera.getTransform() * rotate(radians(cameraRotation.y), vec3(1.0f, 0.0f, 0.0f)));
		glm::vec3 movement(window.isKeyPressed(GLFW_KEY_D) - window.isKeyPressed(GLFW_KEY_A), window.isKeyPressed(GLFW_KEY_W) - window.isKeyPressed(GLFW_KEY_S), 0.0f);
		camera.setTransform(camera.getTransform() * translate(movement * CAMERA_SPEED * (float)deltaTime));

		shader.use();
		shader.setViewMatrix(camera.getViewMatrix());

		vec3 tangent = normalize(spline.direction((float)totalTime));
		vec3 normal = spline.normal((float)totalTime);
		if (normal == vec3(0.0f, 0.0f, 0.0f))
			normal == vec3(0.0f, 1.0f, 0.0f);
		else
			normal = normalize(normal);
		vec3 binormal = normalize(cross(tangent, normal));
		mat3 dcm(tangent, -normal, binormal);
		float angle = (float)acos(tangent.x);
		vec3 rotation(0.0f, -tangent.z, tangent.y);

		shader.setModelMatrix(translate(vec3(5.0f, 0.0f, 0.0f) + spline.position((float)totalTime)) * rotate(angle, rotation));
		mesh.draw();

		shader.setModelMatrix(translate(vec3(-15.0f, 0.0f, 0.0f) + spline.position((float)totalTime)) * mat4(dcm));
		mesh.draw();

		lineShader.use();
		lineShader.setViewMatrix(camera.getViewMatrix());
		lineShader.setModelMatrix(translate(vec3(5.0f, 0.0f, 0.0f)));
		line.draw();
		lineShader.setModelMatrix(translate(vec3(-15.0f, 0.0f, 0.0f)));
		line.draw();

		tangentShader.use();
		tangentShader.setViewMatrix(camera.getViewMatrix());
		tangentShader.setModelMatrix(translate(vec3(5.0f, 0.0f, 0.0f)));
		tangents.draw(false);
		tangentShader.setModelMatrix(translate(vec3(-15.0f, 0.0f, 0.0f)));
		tangents.draw(false);

		window.swapBuffers();
	}

	return 0;
}