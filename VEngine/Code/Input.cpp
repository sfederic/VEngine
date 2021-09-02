#include "Input.h"

namespace Input
{
	std::vector<Keys> currentDownKeys;
	std::vector<Keys> currentUpKeys;

	bool mouseWheelUp;
	bool mouseWheelDown;

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
		mouseRightUp = false;
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
		return currentDownKeys.size();
	}

	bool GetAsyncKey(Keys key)
	{
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
		return mouseLeftUp;
	}

	bool GetMouseRightUp()
	{
		return mouseRightUp;
	}

	bool GetMouseLeftDown()
	{
		return mouseLeftDown;
	}

	bool GetMouseRightDown()
	{
		return mouseRightDown;
	}

	bool GetMouseMiddleUp()
	{
		return mouseMiddleUp;
	}

	bool GetMouseMiddleDown()
	{
		return mouseMiddleDown;
	}
}
