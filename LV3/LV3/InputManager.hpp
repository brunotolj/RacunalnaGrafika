#pragma once
#include <map>

enum class InputMode {
	KeyState,
	CountPresses,
	CountPressesWithRepeats,
	CountReleases
};

class InputManager {

	std::map<int, std::pair<InputMode, int>> keyboardMap;

public:

	InputManager();
	~InputManager();

	int getKeyData(int key);
	void keyPress(int key);
	void keyRelease(int key);
	void keyRepeat(int key);

	void setKeyMode(int key, InputMode mode);
	void removeKeyMode(int key);
};