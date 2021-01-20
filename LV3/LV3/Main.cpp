#include "RenderContext.hpp"
#include "GameSetup.hpp"
#include <chrono>
#include <GLM/gtc/matrix_transform.hpp>

int main() {
	RenderContext window(1920, 1080, true);
	//RenderContext window(1280, 720);
	window.setBackgroundColor(0.8f, 0.4f, 1.0f);

	InputManager input;
	window.setInputManager(input);

	Shader shader;
	shader.compile("vert", "frag");
	shader.setLightIntensity(glm::vec3(0.7f, 0.4f, 0.5f));
	shader.setLightDirection(glm::vec3(-0.6f, -0.5f, 0.4f));

	Camera camera;
	setupCamera(camera);
	shader.setProjectionMatrix(camera.getProjectionMatrix(window.aspectRatio()));
	glm::mat4 cameraBaseTransform = camera.getLocalTransform();

	Mesh boardMesh, tileMesh[3];
	boardMesh.readFromFile("Board");
	tileMesh[0].readFromFile("Virus");
	tileMesh[1].readFromFile("SinglePill");
	tileMesh[2].readFromFile("DoublePill");

	GameBoard boards[2]{ input,input };
	Object tile[2]{ boards[0], boards[1] };
	boards[0].setMesh(boardMesh);
	boards[1].setMesh(boardMesh);

	setDefaultInput(boards);

	glm::mat4 genPos[2];
	calculateTransformMatrices(boards, genPos);

	glm::vec3 color[4]{ glm::vec3(0.6f, 0.2f, 0.8f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.9f, 0.0f), glm::vec3(0.3f, 0.5f, 1.0f) };

	unsigned randSeed = unsigned(time(NULL));
	boards[0].start(randSeed, 5);
	boards[1].start(randSeed, 5);

	double currentTime = glfwGetTime(), lastTime = currentTime, deltaTime = 0.0, virusAltTime = 0.0;
	bool running = true, virusAlt = false;

	float x = 0.0f, y = 0.0f, dx = 0.0f, dy = 0.0f;

	while (!window.shouldClose()) {
		window.pollEvents();

		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		virusAltTime += deltaTime;
		if (virusAltTime >= 0.5) {
			virusAltTime -= 0.5;
			virusAlt = !virusAlt;
		}

		dx += (5.0f * rand() / RAND_MAX - 0.2f * x - 2.5f) * (float)deltaTime;
		dy += (5.0f * rand() / RAND_MAX - 0.2f * y - 2.5f) * (float)deltaTime;
		x += (dx - 0.1f * x) * (float)deltaTime;
		y += (dy - 0.1f * y) * (float)deltaTime;

		camera.setLocalTransform(glm::rotate(cameraBaseTransform, glm::radians(x), glm::vec3(0.0f, 0.0f, 1.0f)));
		camera.setLocalTransform(glm::rotate(camera.getLocalTransform(), glm::radians(y), glm::vec3(1.0f, 0.0f, 0.0f)));
		shader.setViewMatrix(camera.getViewMatrix());

		shader.setObjectColor(color[0]);
		for (int k = 0; k < 2; k++) {
			if (running)
				boards[k].update(deltaTime);
			boards[k].draw(shader);
		}
		for (int k = 0; k < 2; k++) {
			if (boards[k].getState() == BoardState::GameOver) {
				running = false;
				continue;
			}
			
			int genTileData = boards[k].getGenerated(0);
			tile[k].setLocalTransform(genPos[0]);
			configureTile(tile[k], shader, tileMesh, color, genTileData, virusAlt);
			tile[k].draw(shader);
			
			genTileData = boards[k].getGenerated(1);
			tile[k].setLocalTransform(genPos[1]);
			configureTile(tile[k], shader, tileMesh, color, genTileData, virusAlt);
			tile[k].draw(shader);
		}

		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 17; j++) {
				glm::mat4 position = calculateTilePosition(i, j);
				for (int k = 0; k < 2; k++) {
					if (boards[k].getState() == BoardState::GameOver)
						continue;
					int tileData = boards[k].getTile(i, j);
					if (tileData) {
						tile[k].setLocalTransform(position);
						configureTile(tile[k], shader, tileMesh, color, tileData, virusAlt);
						tile[k].draw(shader);
					}
				}
			}
		window.swapBuffers();
	}

	return 0;
}