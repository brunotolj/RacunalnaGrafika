#pragma once
#include "Object.hpp"
#include "InputManager.hpp"

enum class BoardState {
	Initializing,
	Dropping,
	Waiting,
	GameOver
};

enum class InputCommand {
	MoveLeft = 0,
	MoveRight = 1,
	DropFaster = 2,
	RotateLeft = 3,
	RotateRight = 4
};

class GameBoard : public Object {

	BoardState state;
	double waitingTime;
	unsigned tiles[8][17], generated[2], randSeed;
	int x, y, inputKeys[5];
	
	InputManager& inputManager;

	void generateNext();
	bool spawnNext();
	bool drop();
	bool gravity();
	void removeTile(int x, int y);
	bool removeLines();
	void moveLeft();
	void moveRight();
	void rotateLeft();
	void rotateRight();
	bool processInput();

public:

	GameBoard(InputManager& inputManager);
	void start(unsigned seed, int difficulty);

	virtual void update(double deltaTime);
	BoardState getState() const;

	int virusCount() const;
	unsigned getTile(int x, int y) const;
	unsigned getGenerated(bool secondPart) const;
	void setInputKey(InputCommand command, int key);
};