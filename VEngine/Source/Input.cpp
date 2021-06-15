#include "Input.h"

InputSystem gInputSystem;

bool InputSystem::GetAnyKeyDown()
{
	if (keyDown)
	{
		return true;
	}

	return false;
}

bool InputSystem::GetAnyKeyUp()
{
	if (keyUp)
	{
		return true;
	}

	return false;
}

void InputSystem::StoreKeyDownInput(int key)
{
	currentDownKey = key;
	currentDownKeys.push_back(key);
	keyDown = true;
}

void InputSystem::StoreKeyUpInput(int key)
{
	currentUpKey = key;
	currentUpKeys.push_back(key);
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

void InputSystem::StoreMouseMiddleDownInput()
{
	middleMouseDown = true;
	middleMouseUp = false;
}

void InputSystem::StoreMouseMiddleUpInput()
{
	middleMouseUp = true;
	middleMouseDown = false;
}

bool InputSystem::GetKeyDownState(Keys key)
{
	for (int i = 0; i < currentDownKeys.size(); i++)
	{
		if ((int)key == currentDownKeys[i])
		{
			return true;
		}
	}

	return false;
}

bool InputSystem::GetKeyDownState(Keys key, Keys modifier)
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

	return false;
}

bool InputSystem::GetMouseLeftUpState()
{
	if (leftMouseUp)
	{
		return true;
	}
		
	return false;
}

bool InputSystem::GetMouseRightDownState()
{
	if (rightMouseDown)
	{
		return true;
	}

	return false;
}

bool InputSystem::GetMouseRightUpState()
{
	if (rightMouseUp)
	{
		return true;
	}
		
	return false;
}

bool InputSystem::GetMouseMiddleUpState()
{
	if (middleMouseUp)
	{
		return true;
	}

	return false;
}

bool InputSystem::GetMouseMiddleDownState()
{
	if (middleMouseDown)
	{
		return true;
	}

	return false;
}

bool InputSystem::GetAsyncKey(Keys key)
{
	if (GetAsyncKeyState((int)key))
	{
		return true;
	}

	return false;
}

bool InputSystem::GetKeyUpState(Keys key)
{
	for (int i = 0; i < currentUpKeys.size(); i++)
	{
		if ((int)key == currentUpKeys[i])
		{
			return true;
		}
	}

	return false;
}

bool InputSystem::GetKeyUpState(Keys key, Keys modifier)
{
	if (GetAsyncKey(modifier))
	{
		return GetKeyUpState(key);
	}

	return false;
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
	middleMouseUp = false;
	middleMouseDown = false;
	keyDown = false;
	keyUp = false;

	currentDownKey = (int)Keys::None;
	currentUpKey = (int)Keys::None;

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
