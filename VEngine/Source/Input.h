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
	BackSpace = VK_BACK,
	Alt = VK_LMENU, //Left Menu is alt?

	W = 'W',
	A = 'A',
	S = 'S',
	D = 'D',
	F = 'F',
	Z = 'Z',

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
	MiddleMouse = VK_MBUTTON
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
	void StoreMouseMiddleDownInput();
	void StoreMouseMiddleUpInput();

	bool GetKeyUpState(Keys key);
	bool GetKeyUpState(Keys key, Keys modifier);
	bool GetKeyUpState(int key);
	bool GetKeyUpState(int key, int modifier);
	bool GetKeyDownState(Keys key);
	bool GetKeyDownState(Keys key, Keys modifier);
	bool GetKeyDownState(int key);
	bool GetKeyDownState(int key, int modifier);

	bool GetMouseLeftDownState();
	bool GetMouseLeftUpState();
	bool GetMouseRightDownState();
	bool GetMouseRightUpState();
	bool GetMouseMiddleUpState();
	bool GetMouseMiddleDownState();
	bool GetAsyncKey(Keys key);
	bool GetAsyncKey(int key);
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
	bool middleMouseDown;
	bool middleMouseUp;
	bool keyUp;
	bool keyDown;
	bool bMouseWheelUp;
	bool bMouseWheelDown;
};

extern InputSystem gInputSystem;
