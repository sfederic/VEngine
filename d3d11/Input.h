#pragma once

#include <Windows.h>

class InputSystem
{
public:
	bool GetAnyKeyUp();
	bool GetAnyKeyDown();
	void StoreKeyDownInput(WPARAM key);
	void StoreKeyUpInput(WPARAM key);
	void StoreMouseLeftDownInput(WPARAM key);
	void StoreMouseLeftUpInput(WPARAM key);
	void StoreMouseRightDownInput(WPARAM key);
	void StoreMouseRightUpInput(WPARAM key);
	void StoreMouseMiddleDownInput(WPARAM key);
	void StoreMouseMiddleUpInput(WPARAM key);
	bool GetKeyUpState(int key);
	bool GetKeyDownState(int key);
	bool GetMouseLeftDownState();
	bool GetMouseLeftUpState();
	bool GetMouseRightDownState();
	bool GetMouseRightUpState();
	bool GetMouseMiddleUpState();
	bool GetMouseMiddleDownState();
	bool GetAsyncKey(WPARAM key);
	void InputReset();
	void StoreMouseWheelUp();
	void StoreMouseWheelDown();
	bool GetMouseWheelUp();
	bool GetMouseWheelDown();

	WPARAM currentUpKey;
	WPARAM currentDownKey;
	bool leftMouseUp;
	bool leftMouseDown;
	bool rightMouseUp;
	bool rightMouseDown;
	bool keyUp;
	bool keyDown;
	bool bMouseWheelUp;
	bool bMouseWheelDown;
	bool bMiddleMouseDown;
	bool bMiddleMouseUp;
};

extern InputSystem gInputSystem;
