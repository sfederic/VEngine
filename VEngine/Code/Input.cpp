#include "Input.h"
#include <vector>
#include <Windows.h> //@Todo: can probably get rid of this, only needed for GetAsyncKey()

std::vector<Keys> currentDownKeys;
std::vector<Keys> currentUpKeys;

namespace Input
{
	bool mouseWheelUp;
	bool mouseWheelDown;

	bool blockInput = false;

	bool mouseLeftUp;
	bool mouseLeftDown;
	bool mouseRightUp;
	bool mouseRightDown;
	bool mouseMiddleUp;
	bool mouseMiddleDown;

	void Reset()
	{
		currentUpKeys.clear();
		currentDownKeys.clear();

		mouseWheelUp = false;
		mouseWheelDown = false;

		mouseLeftUp = false;
		mouseLeftDown = false;

		mouseRightUp = false;
		mouseRightDown = false;

		mouseMiddleUp = false;
	}

	void SetKeyDown(Keys key)
	{
		currentDownKeys.push_back(key);
	}

	void SetKeyUp(Keys key)
	{
		currentUpKeys.push_back(key);
	}

	bool GetKeyDown(Keys key)
	{
		if (blockInput) return false;

		for (int i = 0; i < currentDownKeys.size(); i++)
		{
			if (key == currentDownKeys[i])
			{
				return true;
			}
		}

		return false;
	}

	bool GetKeyUp(Keys key)
	{
		if (blockInput) return false;

		for (int i = 0; i < currentUpKeys.size(); i++)
		{
			if (key == currentUpKeys[i])
			{
				return true;
			}
		}

		return false;
	}

	bool GetAnyKeyDown()
	{
		if (blockInput) return false;
		return currentDownKeys.size();
	}

	bool GetAsyncKey(Keys key)
	{
		if (blockInput) return false;
		return GetAsyncKeyState((int)key);
	}

	void SetLeftMouseDown()
	{
		mouseLeftDown = true;
		mouseLeftUp = false;
	}

	void SetRightMouseUp()
	{
		mouseRightUp = true;
		mouseRightDown = false;
	}

	void SetRightMouseDown()
	{
		mouseRightDown = true;
		mouseRightUp = false;
	}

	void SetMiddleMouseUp()
	{
		mouseMiddleUp = true;
		mouseMiddleDown = false;
	}

	void SetMiddleMouseDown()
	{
		mouseMiddleDown = true;
		mouseMiddleUp = false;
	}

	void SetLeftMouseUp()
	{
		mouseLeftUp = true;
		mouseLeftDown = false;
	}

	bool GetMouseLeftUp()
	{
		if (blockInput) return false;
		return mouseLeftUp;
	}

	bool GetMouseRightUp()
	{
		if (blockInput) return false;
		return mouseRightUp;
	}

	bool GetMouseLeftDown()
	{
		if (blockInput) return false;
		return mouseLeftDown;
	}

	bool GetMouseRightDown()
	{
		if (blockInput) return false;
		return mouseRightDown;
	}

	bool GetMouseMiddleUp()
	{
		if (blockInput) return false;
		return mouseMiddleUp;
	}

	bool GetMouseMiddleDown()
	{
		if (blockInput) return false;
		return mouseMiddleDown;
	}

	unsigned int GetNumCurrentKeysDown()
	{
		return currentDownKeys.size();
	}

	Keys GetLastPressedKeyDown()
	{
		return currentDownKeys.back();
	}
}
