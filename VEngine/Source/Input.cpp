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

void InputSystem::StoreKeyDownInput(int key)
{
	currentDownKey = key;
	keyDown = true;
}

void InputSystem::StoreKeyUpInput(int key)
{
	currentUpKey = key;
	keyUp = true;
}

void InputSystem::StoreMouseLeftDownInput()
{
	leftMouseDown = true;
	leftMouseUp = false;
}

void InputSystem::StoreMouseLeftUpInput()
{
	leftMouseUp = true;
	leftMouseDown = false;
}

void InputSystem::StoreMouseRightDownInput()
{
	rightMouseDown = true;
	rightMouseUp = false;
}

void InputSystem::StoreMouseRightUpInput()
{
	rightMouseUp = true;
	rightMouseDown = false;
}

bool InputSystem::GetKeyDownState(Keys key)
{
	if (currentDownKey == (int)key)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputSystem::GetKeyDownState(Keys key, Keys modifier)
{
	if (GetAsyncKey(modifier))
	{
		return GetKeyDownState(key);
	}

	return false;
}

bool InputSystem::GetKeyDownState(int key)
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

bool InputSystem::GetKeyDownState(int key, int modifier)
{
	if (GetAsyncKey(modifier))
	{
		return GetKeyDownState(key);
	}

	return false;
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

bool InputSystem::GetAsyncKey(Keys key)
{
	if (GetAsyncKeyState((int)key))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputSystem::GetAsyncKey(int key)
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

bool InputSystem::GetKeyUpState(Keys key)
{
	if (currentUpKey == (int)key)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputSystem::GetKeyUpState(Keys key, Keys modifier)
{
	if (GetAsyncKey(modifier))
	{
		return GetKeyUpState(key);
	}

	return false;
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

bool InputSystem::GetKeyUpState(int key, int modifier)
{
	if (GetAsyncKey(modifier))
	{
		return GetKeyUpState(key);
	}

	return false;
}

void InputSystem::InputReset()
{
	rightMouseDown = false;
	rightMouseUp = false;
	leftMouseDown = false;
	leftMouseUp = false;
	currentDownKey = (int)Keys::None;
	currentUpKey = (int)Keys::None;
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
