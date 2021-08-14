#include "Input.h"

std::vector<Keys> Input::currentDownKeys;
std::vector<Keys> Input::currentUpKeys;

bool Input::mouseWheelUp;
bool Input::mouseWheelDown;

void Input::Reset()
{
	currentUpKeys.clear();
	currentDownKeys.clear();
	mouseWheelUp = false;
	mouseWheelDown = false;
}

void Input::SetKeyDown(Keys key)
{
	currentDownKeys.push_back(key);
}

void Input::SetKeyUp(Keys key)
{
	currentUpKeys.push_back(key);
}

bool Input::GetKeyDown(Keys key)
{
	for (int i = 0; i < currentDownKeys.size(); i++)
	{
		if (key == currentDownKeys[i])
		{
			return true;
		}
	}

	return false;
}

bool Input::GetKeyUp(Keys key)
{
	for (int i = 0; i < currentUpKeys.size(); i++)
	{
		if (key == currentUpKeys[i])
		{
			return true;
		}
	}

	return false;
}

bool Input::GetAnyKeyDown()
{
	return currentDownKeys.size();
}

bool Input::GetAsyncKey(Keys key)
{
	return GetAsyncKeyState((int)key);
}
