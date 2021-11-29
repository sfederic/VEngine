#pragma once
#include <vector>
#include <Windows.h>

enum class Keys
{
	None = 0,
	Space = VK_SPACE,
	Enter = VK_RETURN,
	Esc = VK_ESCAPE,
	Tab = VK_TAB,
	Tilde = VK_OEM_3,
	Ctrl = VK_CONTROL,
	ShiftLeft = VK_LSHIFT,
	ShiftRight = VK_RSHIFT,
	Delete = VK_DELETE,
	BackSpace = VK_BACK,
	Alt = VK_LMENU,
	//TODO: there's all sorts of scancode shit win Win32 and keyboards
	//that won't make this work internationally.
	W = 'W',
	A = 'A',
	S = 'S',
	D = 'D',
	F = 'F',
	Z = 'Z',
	X = 'X',
	P = 'P',
	E = 'E',
	R = 'R',
	O = 'O',
	G = 'G',
	V = 'V',
	B = 'B',
	Q = 'Q',
	T = 'T',
	C = 'C',
	I = 'I',
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
	F1 = VK_F1,
	F2 = VK_F2,
	F3 = VK_F3,
	F4 = VK_F4,
	F11 = VK_F11,
	Up = VK_UP,
	Down = VK_DOWN,
	Right = VK_RIGHT,
	Left = VK_LEFT,
	MouseLeft = VK_LBUTTON,
	MouseRight = VK_RBUTTON,
	MouseMiddle = VK_MBUTTON
};

namespace Input
{
	extern std::vector<Keys> currentDownKeys;
	extern std::vector<Keys> currentUpKeys;

	extern bool mouseWheelUp;
	extern bool mouseWheelDown;

	void Reset();

	void SetKeyDown(Keys key);
	void SetKeyUp(Keys key);

	bool GetKeyDown(Keys key);
	bool GetKeyUp(Keys key);
	bool GetAnyKeyDown();
	bool GetAsyncKey(Keys key);

	void SetLeftMouseUp();
	void SetLeftMouseDown();
	void SetRightMouseUp();
	void SetRightMouseDown();
	void SetMiddleMouseUp();
	void SetMiddleMouseDown();

	bool GetMouseLeftUp();
	bool GetMouseRightUp();
	bool GetMouseLeftDown();
	bool GetMouseRightDown();
	bool GetMouseMiddleUp();
	bool GetMouseMiddleDown();
}
