#pragma once

#include <Windows.h>
#include <stdint.h>

#ifdef WIN32
enum class Keys : uint8_t
{
	None = 0,
	Space = VK_SPACE,
	W = 'W',
	A = 'A',
	S = 'S',
	D = 'D',
	Enter = VK_RETURN,
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
	Tab = VK_TAB,
};
#endif

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

	bool GetKeyUpState(int key);
	bool GetKeyDownState(int key);

	bool GetMouseLeftDownState();
	bool GetMouseLeftUpState();
	bool GetMouseRightDownState();
	bool GetMouseRightUpState();
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
};

extern InputSystem gInputSystem;
