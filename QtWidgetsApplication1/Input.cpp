#include "Input.h"

InputSystem gInputSystem;

bool InputSystem::GetAnyKeyUp()
{
	if (keyUp)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputSystem::GetAnyKeyDown()
{
	if (keyDown)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void InputSystem::StoreKeyDownInput(WPARAM key)
{
	currentDownKey = key;
	keyDown = true;
}

void InputSystem::StoreKeyUpInput(WPARAM key)
{
	currentUpKey = key;
	keyUp = true;
}

void InputSystem::StoreMouseLeftDownInput(WPARAM key)
{
	leftMouseDown = true;
	leftMouseUp = false;
}

void InputSystem::StoreMouseLeftUpInput(WPARAM key)
{
	leftMouseUp = true;
	leftMouseDown = false;
}

void InputSystem::StoreMouseRightDownInput(WPARAM key)
{
	rightMouseDown = true;
	rightMouseUp = false;
}

void InputSystem::StoreMouseRightUpInput(WPARAM key)
{
	rightMouseUp = true;
	rightMouseDown = false;
}

bool InputSystem::GetKeyDownState(int key)
{
	if (currentDownKey == key)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputSystem::GetMouseLeftDownState()
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

bool InputSystem::GetMouseLeftUpState()
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

bool InputSystem::GetMouseRightDownState()
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

bool InputSystem::GetMouseRightUpState()
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

bool InputSystem::GetAsyncKey(WPARAM key)
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

bool InputSystem::GetKeyUpState(int key)
{
	if (currentUpKey == key)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void InputSystem::InputReset()
{
	rightMouseDown = false;
	rightMouseUp = false;
	leftMouseDown = false;
	leftMouseUp = false;
	currentDownKey = 0;
	currentUpKey = 0;
	keyDown = false;
	keyUp = false;

	bMouseWheelDown = false;
	bMouseWheelUp = false;
}

void InputSystem::StoreMouseWheelUp()
{
	bMouseWheelUp = true;
}

void InputSystem::StoreMouseWheelDown()
{
	bMouseWheelDown = true;
}

bool InputSystem::GetMouseWheelUp()
{
	return bMouseWheelUp;
}

bool InputSystem::GetMouseWheelDown()
{
	return bMouseWheelDown;
}
