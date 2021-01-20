#include "InputManager.hpp"
#include <GLFW/glfw3.h>

InputManager::InputManager() {}

InputManager::~InputManager() {}

int InputManager::getKeyData(int key) {
	if (keyboardMap.find(key) != keyboardMap.end()) {
		InputMode mode = keyboardMap[key].first;
		int data = keyboardMap[key].second;
		if (mode == InputMode::CountPresses || mode == InputMode::CountPressesWithRepeats || mode == InputMode::CountReleases)
			keyboardMap[key].second = 0;
		return data;
	}
	throw 1;
}

void InputManager::keyPress(int key) {
	if (keyboardMap.find(key) != keyboardMap.end()) {
		InputMode mode = keyboardMap[key].first;
		int& data = keyboardMap[key].second;
		switch (mode) {

		case InputMode::KeyState:
			data = 1;
			break;

		case InputMode::CountPresses:
		case InputMode::CountPressesWithRepeats:
			data++;
			break;
		}
	}
}

void InputManager::keyRelease(int key) {
	if (keyboardMap.find(key) != keyboardMap.end()) {
		InputMode mode = keyboardMap[key].first;
		int& data = keyboardMap[key].second;
		switch (mode) {

		case InputMode::KeyState:
			data = 0;
			break;

		case InputMode::CountReleases:
			data++;
			break;
		}
	}
}

void InputManager::keyRepeat(int key) {
	if (keyboardMap.find(key) != keyboardMap.end()) {
		InputMode mode = keyboardMap[key].first;
		int& data = keyboardMap[key].second;
		switch (mode) {

		case InputMode::CountPressesWithRepeats:
			data++;
			break;
		}
	}
}

void InputManager::setKeyMode(int key, InputMode mode) {
	keyboardMap[key] = std::make_pair(mode, 0);
}

void InputManager::removeKeyMode(int key) {
	keyboardMap.erase(key);
}