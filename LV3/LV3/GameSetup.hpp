#pragma once
#include "Camera.hpp"
#include "GameBoard.hpp"

void configureTile(Object& tile, Shader& shader, Mesh* meshes, glm::vec3* colors, int data, bool animation);

void setupCamera(Camera& camera);

void setDefaultInput(GameBoard* boards);

void calculateTransformMatrices(GameBoard* boards, glm::mat4* genPos);

glm::mat4 calculateTilePosition(int x, int y);