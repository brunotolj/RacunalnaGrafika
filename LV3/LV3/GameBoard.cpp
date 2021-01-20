#include "GameBoard.hpp"

#include <iostream>

GameBoard::GameBoard(InputManager& inputManager)
	: state(BoardState::GameOver), tiles{}, generated{}, randSeed(0), x(3), y(15), inputKeys{}, waitingTime(0.0), inputManager(inputManager) {}

void GameBoard::generateNext() {
	int c1, c2;
	srand(randSeed);
	do { c1 = rand() & 3; } while (c1 == 3);
	do { c2 = rand() & 3; } while (c2 == 3);
	randSeed = rand();
	generated[0] = ((c1 + 1) << 3) + 4;
	generated[1] = ((c2 + 1) << 3) + 6;
}

bool GameBoard::spawnNext() {
	if (tiles[3][15] || tiles[4][15])
		return false;
	tiles[3][15] = generated[0];
	tiles[4][15] = generated[1];
	x = 3, y = 15;
	generateNext();
	return true;
}

bool GameBoard::drop() {
	if (y == 0 || tiles[x][y - 1])
		return false;
	if (tiles[x][y] & 1) {
		tiles[x][y - 1] = tiles[x][y];
		tiles[x][y] = tiles[x][y + 1];
		tiles[x][y + 1] = 0;
	}
	else {
		if (tiles[x + 1][y - 1])
			return false;
		tiles[x][y - 1] = tiles[x][y];
		tiles[x + 1][y - 1] = tiles[x + 1][y];
		tiles[x][y] = 0;
		tiles[x + 1][y] = 0;
	}
	y--;
	return true;
}

bool GameBoard::gravity() {
	bool flag = false;
	for (int i = 0; i < 8; i++)
		for (int j = 1; j < 17; j++) {
			if (!tiles[i][j])
				continue;
			switch (tiles[i][j] & 7) {
				
			case 1:
				if (!tiles[i][j - 1]) {
					tiles[i][j - 1] = tiles[i][j];
					tiles[i][j] = 0;
					flag = true;
				}
				break;

			case 4:
				if (!tiles[i][j - 1] && !tiles[i + 1][j - 1]) {
					tiles[i][j - 1] = tiles[i][j];
					tiles[i][j] = 0;
					tiles[i + 1][j - 1] = tiles[i + 1][j];
					tiles[i + 1][j] = 0;
					flag = true;
				}
				break;

			case 5:
				if (!tiles[i][j - 1]) {
					tiles[i][j - 1] = tiles[i][j];
					tiles[i][j] = tiles[i][j + 1];
					tiles[i][j + 1] = 0;
					flag = true;
				}
				break;

			}
		}
	return flag;
}

void GameBoard::removeTile(int x, int y) {
	switch (tiles[x][y] & 7) {
	case 4:
		tiles[x + 1][y] = ((tiles[x + 1][y] >> 3) << 3) + 1;
		break;
	case 5:
		tiles[x][y + 1] = ((tiles[x][y + 1] >> 3) << 3) + 1;
		break;
	case 6:
		tiles[x - 1][y] = ((tiles[x - 1][y] >> 3) << 3) + 1;
		break;
	case 7:
		tiles[x][y - 1] = ((tiles[x][y - 1] >> 3) << 3) + 1;
		break;
	}
	tiles[x][y] = ((tiles[x][y] >> 3) << 3) + 2;
}

bool GameBoard::removeLines() {
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 17; j++) {
			if (!tiles[i][j])
				continue;

			int color = tiles[i][j] >> 3, counter = 0;
			for (int k = i + 1; k < 8; k++)
				if ((tiles[k][j] >> 3) == color)
					counter++;
				else
					break;
			if (counter >= 3)
				for (int k = i; k <= i + counter; k++)
					removeTile(k, j);

			counter = 0;
			for (int k = j + 1; k < 17; k++)
				if ((tiles[i][k] >> 3) == color)
					counter++;
				else
					break;
			if (counter >= 3)
				for (int k = j; k <= j + counter; k++)
					removeTile(i, k);
		}
	bool removed = false;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 17; j++)
			if ((tiles[i][j] & 7) == 2) {
				tiles[i][j] = 0;
				removed = true;
			}
	return removed;
}

void GameBoard::moveLeft() {
	if (tiles[x][y] & 1) {
		if (x > 0 && !tiles[x - 1][y] && !tiles[x - 1][y + 1]) {
			tiles[x - 1][y] = tiles[x][y];
			tiles[x][y] = 0;
			tiles[x - 1][y + 1] = tiles[x][y + 1];
			tiles[x][y + 1] = 0;
			x--;
		}
	}
	else {
		if (x > 0 && !tiles[x - 1][y]) {
			tiles[x - 1][y] = tiles[x][y];
			tiles[x][y] = tiles[x + 1][y];
			tiles[x + 1][y] = 0;
			x--;
		}
	}
}

void GameBoard::moveRight() {
	if (tiles[x][y] & 1) {
		if (x < 7 && !tiles[x + 1][y] && !tiles[x + 1][y + 1]) {
			tiles[x + 1][y] = tiles[x][y];
			tiles[x][y] = 0;
			tiles[x + 1][y + 1] = tiles[x][y + 1];
			tiles[x][y + 1] = 0;
			x++;
		}
	}
	else {
		if (x < 6 && !tiles[x + 2][y]) {
			tiles[x + 2][y] = tiles[x + 1][y];
			tiles[x + 1][y] = tiles[x][y];
			tiles[x][y] = 0;
			x++;
		}
	}
}

void GameBoard::rotateLeft() {
	if (tiles[x][y] & 1) {
		if (x < 7 && !tiles[x + 1][y]) {
			tiles[x + 1][y] = tiles[x][y] + 1;
			tiles[x][y] = tiles[x][y + 1] - 3;
			tiles[x][y + 1] = 0;
		}
		else if (x > 0 && !tiles[x - 1][y]) {
			tiles[x - 1][y] = tiles[x][y + 1] - 3;
			tiles[x][y + 1] = 0;
			tiles[x][y]++;
			x--;
		}
	}
	else {
		if (!tiles[x][y + 1]) {
			tiles[x][y + 1] = tiles[x + 1][y] + 1;
			tiles[x + 1][y] = 0;
			tiles[x][y]++;
		}
		else if (!tiles[x + 1][y + 1]) {
			tiles[x + 1][y + 1] = tiles[x + 1][y] + 1;
			tiles[x + 1][y] = tiles[x][y] + 1;
			tiles[x][y] = 0;
			x++;
		}
	}
}

void GameBoard::rotateRight() {
	if (tiles[x][y] & 1) {
		if (x < 7 && !tiles[x + 1][y]) {
			tiles[x + 1][y] = tiles[x][y + 1] - 1;
			tiles[x][y + 1] = 0;
			tiles[x][y]--;
		}
		else if (x > 0 && !tiles[x - 1][y]) {
			tiles[x - 1][y] = tiles[x][y] - 1;
			tiles[x][y] = tiles[x][y + 1] - 1;
			tiles[x][y + 1] = 0;
			x--;
		}
	}
	else {
		if (!tiles[x][y + 1]) {
			tiles[x][y + 1] = tiles[x][y] + 3;
			tiles[x][y] = tiles[x + 1][y] - 1;
			tiles[x + 1][y] = 0;
		}
		else if (!tiles[x + 1][y + 1]) {
			tiles[x + 1][y + 1] = tiles[x][y] + 3;
			tiles[x][y] = 0;
			tiles[x + 1][y]--;
			x++;
		}
	}
}

bool GameBoard::processInput() {
	int key = inputKeys[size_t(InputCommand::MoveLeft)], keyData;
	if (key) {
		keyData = inputManager.getKeyData(key);
		while (keyData--) { moveLeft(); }
	}
	key = inputKeys[size_t(InputCommand::MoveRight)];
	if (key) {
		keyData = inputManager.getKeyData(key);
		while (keyData--) { moveRight(); }
	}
	key = inputKeys[size_t(InputCommand::RotateLeft)];
	if (key) {
		keyData = inputManager.getKeyData(key);
		while (keyData--) { rotateLeft(); }
	}
	key = inputKeys[size_t(InputCommand::RotateRight)];
	if (key) {
		keyData = inputManager.getKeyData(key);
		while (keyData--) { rotateRight(); }
	}
	bool dropped = false;
	key = inputKeys[size_t(InputCommand::DropFaster)];
	if (key && inputManager.getKeyData(key))
		dropped = drop();
	return dropped;
}

void GameBoard::start(unsigned seed, int difficulty) {
	difficulty *= 4;
	srand(seed);
	while (virusCount() < difficulty) {
		int xpos = rand() & 7, ypos = (rand() & 7) + (rand() & 3), c;
		if (tiles[xpos][ypos])
			continue;
		do { c = rand() & 3; } while (c == 3);
		tiles[xpos][ypos] = (c + 1) << 3;
		removeLines();
	}
	randSeed = rand();
	state = BoardState::Initializing;
	waitingTime = 1.0;
	generateNext();
}

void GameBoard::update(double deltaTime) {
	if (state == BoardState::GameOver)
		return;

	bool dropped = false;
	if (state == BoardState::Dropping)
		dropped = processInput();

	while (waitingTime <= deltaTime) {
		deltaTime -= waitingTime;
		switch (state) {

		case BoardState::Initializing:
			spawnNext();
			waitingTime = 0.5;
			state = BoardState::Dropping;
			break;

		case BoardState::Waiting:
			if (gravity() || removeLines())
				waitingTime = 0.1;
			else if (spawnNext()) {
				waitingTime = 0.5;
				state = BoardState::Dropping;
			}
			else {
				state = BoardState::GameOver;
				return;
			}
			break;

		case BoardState::Dropping:
			if (dropped || drop()) {
				waitingTime = 0.5;
			}
			else {
				waitingTime = 0.1;
				state = BoardState::Waiting;
			}
			break;

		}
	}
	waitingTime -= deltaTime;
}

BoardState GameBoard::getState() const { return state; }

int GameBoard::virusCount() const {
	int count = 0;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 16; j++)
			if (tiles[i][j] && !(tiles[i][j] & 7))
				count++;
	return count;
}

unsigned GameBoard::getTile(int x, int y) const {
	return tiles[x][y];
}

unsigned GameBoard::getGenerated(bool secondPart) const {
	return generated[secondPart];
}

void GameBoard::setInputKey(InputCommand command, int key) {
	size_t index = size_t(command);
	if (inputKeys[index])
		inputManager.removeKeyMode(inputKeys[index]);
	inputKeys[index] = key;
	if (key)
		inputManager.setKeyMode(key, (command == InputCommand::DropFaster) ? InputMode::KeyState : InputMode::CountPressesWithRepeats);
}