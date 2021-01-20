#include "RenderContext.hpp"
#include "LineMesh.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Particle.hpp"
#include <GLM/gtx/transform.hpp>
#include <fstream>
using namespace std;
using namespace glm;

unsigned WIDTH = 1920, HEIGHT = 1080;
float CAMERA_SPEED = 5.0f, ANIMATION_SPEED = 1.0f;
bool FULLSCREEN = true;

unsigned PARTICLE_MAX_COUNT = 8000;
float PARTICLE_LIFE = 1.5f;
glm::vec3 START_POSITION(0.0f, 0.0f, 0.0f);
glm::vec3 START_VELOCITY(0.0f, 0.0f, 0.2f);
glm::vec3 FORCE(0.0f, 0.0f, 1.1f);

int main()
{
	ifstream config("cfg.txt");
	if (config.is_open())
	{
		config >> WIDTH;
		config >> HEIGHT;
		config >> FULLSCREEN;
		config >> CAMERA_SPEED;
		config >> ANIMATION_SPEED;
		config >> PARTICLE_MAX_COUNT;
		config >> PARTICLE_LIFE;
	}
	RenderContext window(WIDTH, HEIGHT, FULLSCREEN);
	window.setBackgroundColor(0.1f, 0.1f, 0.1f);

	vec2 cameraRotation(0.0f);
	window.setMouseTarget(cameraRotation);

	Camera camera;
	camera.setTransform(translate(vec3(0.0f, -3.0f, 1.0f)));

	Shader particleShader;
	particleShader.compile("particlevert", "particlefrag");
	particleShader.setProjectionMatrix(camera.getProjectionMatrix(window.aspectRatio()));
	particleShader.setFactor(800.0f / PARTICLE_MAX_COUNT);

	double lastTime = glfwGetTime(), currentTime, deltaTime, totalTime = 0.0;

	std::vector<Particle> particles(PARTICLE_MAX_COUNT);
	double newParticleFrequency = PARTICLE_MAX_COUNT / PARTICLE_LIFE, particleSpawnCounter = 0.0;

	Mesh particleMesh;
	particleMesh.createParticle();

	Texture fireTexture("Textures\\fire.png");

	Spline spline;
	spline.readFromFile("spline");

	while (!window.shouldClose())
	{
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		totalTime += deltaTime * ANIMATION_SPEED;
		if (totalTime >= spline.numberOfSegments())
			totalTime -= spline.numberOfSegments();

		particleSpawnCounter += deltaTime * newParticleFrequency;
		unsigned newParticles = unsigned(particleSpawnCounter);
		particleSpawnCounter -= newParticles;

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

		particleShader.setViewMatrix(camera.getViewMatrix());

		glm::vec3 deltaTimeVector((float)deltaTime);

		for (unsigned int i = 0; i < newParticles; i++)
		{
			unsigned deadParticle = FindDeadParticle(particles);
			if (deadParticle < particles.size())
				RespawnParticle(
					particles[deadParticle],
					START_POSITION + spline.position(float(totalTime)),
					START_VELOCITY - glm::vec3(0.5f * ANIMATION_SPEED) * spline.direction(float(totalTime)),
					PARTICLE_LIFE);
		}

		for (unsigned int i = 0; i < particles.size(); ++i)
		{
			Particle& p = particles[i];
			p.Life -= deltaTime;
			if (p.Life > 0.0f) {
				p.Velocity += (p.Force + FORCE) * deltaTimeVector;
				p.Position += p.Velocity * deltaTimeVector;
			}
		}
		particleMesh.draw(particles, particleShader.view);

		window.swapBuffers();
	}

	return 0;
}