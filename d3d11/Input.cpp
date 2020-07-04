#include "Input.h"

WPARAM currentDownKey;
WPARAM currentUpKey;
bool mouseUp;
bool mouseDown;
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

void StoreMouseDownInput(WPARAM key)
{
	mouseDown = true;
	mouseUp = false;
}

void StoreMouseUpInput(WPARAM key)
{
	mouseUp = true;
	mouseDown = false;
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

bool GetMouseDownState()
{
	if (mouseDown)
	{
		mouseDown = false;
		return true;
	}
	else
	{
		return false;
	}
}

bool GetMouseUpState()
{
	if (mouseUp)
	{
		mouseUp = false;
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
