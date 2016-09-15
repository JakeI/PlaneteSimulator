#pragma once

#define INPUT_CHANGE_BUFFER int
#define CHANGE_WINDOW_SIZE			(INPUT_CHANGE_BUFFER)(2)
#define CHANGE_MOUSE_CORDINATES		(INPUT_CHANGE_BUFFER)(4)
#define CHANGE_MOUSE_BUTTON_STATE	(INPUT_CHANGE_BUFFER)(8)
#define CHANGE_KEYBOARD_STATE		(INPUT_CHANGE_BUFFER)(16)
#define CHANGE_MOUSEWHEEL_STATE		(INPUT_CHANGE_BUFFER)(32)

struct UserInput {
	struct WindowSize {
		int width;
		int hight;
		WindowSize();
	} windowSize;

	struct MouseCordinates {
		int x;
		int y;
		MouseCordinates();
	}mouseCordinates;

	struct MouseButtonStates {
		bool rightButtonPressed;
		bool leftButtonPressed;
		MouseButtonStates();
	}mouseButtoneStates;

	class KeyboardState {
	private:
		static const int bitPint = 8 * sizeof(int);
		int data[256 / bitPint];
	public:
		KeyboardState();
		void set(int id, bool value);
		void setFalse(int id);
		void setTrue(int id);
		bool operator[](int id);
	} keyboardState;

	float mouseWheel;//in lies not in max Wheel;

	UserInput();
};
