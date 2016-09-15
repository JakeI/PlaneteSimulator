#include "UserInput.h"

UserInput::WindowSize::WindowSize() {
	width = 0;
	hight = 0;
}

UserInput::MouseCordinates::MouseCordinates() {
	x = 0;
	y = 0;
}

UserInput::MouseButtonStates::MouseButtonStates() {
	leftButtonPressed = false;
	rightButtonPressed = false;
}

UserInput::KeyboardState::KeyboardState() {
	for (int i = 0, end = 256 / bitPint; i < end; i++) {
		data[i] = 0;
	}
}

void UserInput::KeyboardState::set(int id, bool value) {
	if (value)
		setTrue(id);
	else
		setFalse(id);
}

void UserInput::KeyboardState::setFalse(int id) {
	// 1010 & 1101 == 1000
	int block = id / bitPint;
	data[block] &= ~(1 << (id % bitPint));
}

void UserInput::KeyboardState::setTrue(int id) {
	// 1010 | 0010 == 1010
	// 1000 | 0010 == 1010
	int block = id / bitPint;
	data[block] |= (1 << (id % bitPint));
}

bool UserInput::KeyboardState::operator[](int id) {//get value
	int block = id / bitPint;
	return data[block] & (1 << (id % bitPint));
}

UserInput::UserInput() {
	mouseWheel = 0.0f;
}