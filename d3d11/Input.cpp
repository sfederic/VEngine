#include "Input.h"

WPARAM currentDownKey;
WPARAM currentUpKey;
bool leftMouseUp;
bool rightMouseUp;
bool leftMouseDown;
bool rightMouseDown;
bool keyDown;
bool keyUp;

bool GetAnyKeyUp()
{
	if (keyUp)
	{
		keyUp = false;
		return true;
	}
	else
	{
		return false;
	}
}

bool GetAnyKeyDown()
{
	if (keyDown)
	{
		keyDown = false;
		return true;
	}
	else
	{
		return false;
	}
}

void StoreKeyDownInput(WPARAM key)
{
	currentDownKey = key;
	keyDown = true;
}

void StoreKeyUpInput(WPARAM key)
{
	currentUpKey = key;
	keyUp = true;
}

void StoreMouseLeftDownInput(WPARAM key)
{
	leftMouseDown = true;
	leftMouseUp = false;
}

void StoreMouseLeftUpInput(WPARAM key)
{
	leftMouseUp = true;
	leftMouseDown = false;
}

void StoreMouseRightDownInput(WPARAM key)
{
	rightMouseDown = true;
	rightMouseUp = false;
}

void StoreMouseRightUpInput(WPARAM key)
{
	rightMouseUp = true;
	rightMouseDown = false;
}

bool GetKeyDownState(int key)
{
	if (currentDownKey == key)
	{
		currentDownKey = 0;
		return true;
	}
	else
	{
		return false;
	}
}

bool GetMouseLeftDownState()
{
	if (leftMouseDown)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GetMouseLeftUpState()
{
	if (leftMouseUp)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GetMouseRightDownState()
{
	if (rightMouseDown)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GetMouseRightUpState()
{
	if (rightMouseUp)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GetAsyncKey(WPARAM key)
{
	if (GetAsyncKeyState(key))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GetKeyUpState(int key)
{
	if (currentUpKey == key)
	{
		currentUpKey = 0;
		return true;
	}
	else
	{
		return false;
	}
}

//Two consecutive mouseups for example were reseting. Had to throw this in before msg pump
//TODO: throw in an input mananger
void InputEnd()
{
	rightMouseDown = false;
	rightMouseUp = false;
	leftMouseDown = false;
	leftMouseUp = false;
	currentDownKey = 0;
	currentUpKey = 0;
	keyDown = false;
	keyUp = false;
}
