#pragma once

#include <Windows.h>
#include <stdint.h>

#ifdef WIN32
enum class Keys
{
	None = 0,
	Space = VK_SPACE,
	Enter = VK_RETURN,
	Tab = VK_TAB,
	Tilde = VK_OEM_3,
	Ctrl = VK_CONTROL,
	Delete = VK_DELETE,

	W = 'W',
	A = 'A',
	S = 'S',
	D = 'D',
	F = 'F',

	_1 = '1',
	_2 = '2',
	_3 = '3',
	_4 = '4',
	_5 = '5',
	_6 = '6',
	_7 = '7',
	_8 = '8',
	_9 = '9',
	_0 = '0',

	Up = VK_UP,
	Down = VK_DOWN,
	Right = VK_RIGHT,
	Left = VK_LEFT,

	LeftMouse = VK_LBUTTON,
	RightMouse = VK_RBUTTON,
};
#endif

class InputSystem
{
public:
	bool GetAnyKeyUp();
	bool GetAnyKeyDown();
	void StoreKeyDownInput(int key);
	void StoreKeyUpInput(int key);
	void StoreMouseLeftDownInput();
	void StoreMouseLeftUpInput();
	void StoreMouseRightDownInput();
	void StoreMouseRightUpInput();

	bool GetKeyUpState(Keys key);
	bool GetKeyUpState(int key);
	bool GetKeyDownState(Keys key);
	bool GetKeyDownState(int key);

	bool GetMouseLeftDownState();
	bool GetMouseLeftUpState();
	bool GetMouseRightDownState();
	bool GetMouseRightUpState();
	bool GetAsyncKey(Keys key);
	void InputReset();
	void StoreMouseWheelUp();
	void StoreMouseWheelDown();
	bool GetMouseWheelUp();
	bool GetMouseWheelDown();

	int currentUpKey;
	int currentDownKey;

	bool leftMouseUp;
	bool leftMouseDown;
	bool rightMouseUp;
	bool rightMouseDown;
	bool keyUp;
	bool keyDown;
	bool bMouseWheelUp;
	bool bMouseWheelDown;
};

extern InputSystem gInputSystem;
