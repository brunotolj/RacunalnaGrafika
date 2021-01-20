#include "GameSetup.hpp"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

void configureTile(Object& tile, Shader& shader, Mesh* meshes, glm::vec3* colors, int data, bool animation) {
	if (data) {
		shader.setObjectColor(colors[data >> 3]);
		if ((data & 7) == 0) {
			tile.setMesh(meshes[0]);
			if (animation)
				tile.setLocalTransform(glm::rotate(tile.getLocalTransform(), glm::radians(22.5f), glm::vec3(0.0f, 0.0f, 1.0f)));
		}
		else if ((data & 4) == 0)
			tile.setMesh(meshes[1]);
		else {
			tile.setMesh(meshes[2]);
			tile.setLocalTransform(glm::rotate(tile.getLocalTransform(), glm::radians(90.0f * (data & 3)), glm::vec3(0.0f, 0.0f, 1.0f)));
		}
	}
}

void setupCamera(Camera& camera) {
	camera.setFOV(60.0f);
	camera.setLocalTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.33f, 0.0f)));
	camera.setLocalTransform(glm::rotate(camera.getLocalTransform(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
}

void setDefaultInput(GameBoard* boards) {
	boards[0].setInputKey(InputCommand::MoveLeft, GLFW_KEY_A);
	boards[0].setInputKey(InputCommand::MoveRight, GLFW_KEY_D);
	boards[0].setInputKey(InputCommand::DropFaster, GLFW_KEY_S);
	boards[0].setInputKey(InputCommand::RotateLeft, GLFW_KEY_N);
	boards[0].setInputKey(InputCommand::RotateRight, GLFW_KEY_M);

	boards[1].setInputKey(InputCommand::MoveLeft, GLFW_KEY_LEFT);
	boards[1].setInputKey(InputCommand::MoveRight, GLFW_KEY_RIGHT);
	boards[1].setInputKey(InputCommand::DropFaster, GLFW_KEY_DOWN);
	boards[1].setInputKey(InputCommand::RotateLeft, GLFW_KEY_KP_1);
	boards[1].setInputKey(InputCommand::RotateRight, GLFW_KEY_KP_2);
}

void calculateTransformMatrices(GameBoard* boards, glm::mat4* genPos) {
	boards[0].setLocalTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, 0.0f, 0.8f)));
	boards[0].setLocalTransform(glm::rotate(boards[0].getLocalTransform(), glm::radians(195.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	boards[1].setLocalTransform(glm::translate(glm::mat4(1.0f), glm::vec3(-0.3f, 0.0f, 0.8f)));
	boards[1].setLocalTransform(glm::rotate(boards[1].getLocalTransform(), glm::radians(165.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	genPos[0] = glm::translate(glm::mat4(1.0f), glm::vec3(-0.02f, 0.71f, 0.0f));
	genPos[1] = glm::translate(glm::mat4(1.0f), glm::vec3(0.02f, 0.71f, 0.0f));
}

glm::mat4 calculateTilePosition(int x, int y) {
	return glm::translate(glm::mat4(1.0f), glm::vec3(-0.14f + 0.04f * x, 0.03f + 0.04f * y, 0.0f));
}