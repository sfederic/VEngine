#pragma once

//All the 0x codes here are from the Win32 virtual key values. They're like this to avoid including Windows.h here.
enum class Keys
{
	None = 0,
	Space = 0x20,
	Enter = 0x0D,
	Esc = 0x1B,
	Tab = 0x09,
	Tilde = 0xC0,
	Ctrl = 0x11,
	ShiftLeft = 0xA0,
	ShiftRight = 0xA1,
	Delete = 0x2E,
	BackSpace = 0x08,
	Alt = 0xA4,
	//@Todo: there's all sorts of scancode shit win Win32 and keyboards
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
	F1 = 0x70,
	F2 = 0x71,
	F3 = 0x72,
	F4 = 0x73,
	F8 = 0x77,
	F11 = 0x7A,
	Up = 0x26,
	Down = 0x28,
	Right = 0x27,
	Left = 0x25,
	MouseLeft = 0x01,
	MouseRight = 0x02,
	MouseMiddle = 0x04
};

namespace Input
{
	extern bool mouseWheelUp;
	extern bool mouseWheelDown;

	extern bool blockInput;

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

	unsigned int GetNumCurrentKeysDown();
	Keys GetLastPressedKeyDown();
}
