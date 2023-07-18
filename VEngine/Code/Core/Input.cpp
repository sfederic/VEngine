#include "vpch.h"
#include "Input.h"

std::set<Keys> currentHeldKeys;
std::set<Keys> currentDownKeys;
std::set<Keys> currentUpKeys;

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
		mouseRightUp = false;
		mouseMiddleUp = false;
	}

	void ResetHeldKeys()
	{
		currentHeldKeys.clear();
	}

	void SetKeyDown(Keys key)
	{
		currentDownKeys.insert(key);
		currentHeldKeys.insert(key);
		currentUpKeys.erase(key);
	}

	void SetKeyUp(Keys key)
	{
		currentUpKeys.insert(key);
		currentDownKeys.erase(key);
		currentHeldKeys.erase(key);
	}

	bool GetKeyDown(Keys key)
	{
		if (blockInput) return false;
		return currentDownKeys.find(key) != currentDownKeys.end();
	}

	bool GetKeyUp(Keys key)
	{
		if (blockInput) return false;
		return currentUpKeys.find(key) != currentUpKeys.end();
	}

	bool GetKeyHeld(Keys key)
	{
		if (blockInput) return false;
		return currentHeldKeys.find(key) != currentHeldKeys.end();
	}

	bool GetAnyKeyDown()
	{
		if (blockInput) return false;
		return currentDownKeys.size();
	}

	bool GetAnyKeyUp()
	{
		if (blockInput) return false;
		return currentUpKeys.size();
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

	size_t GetNumCurrentKeysDown()
	{
		return currentDownKeys.size();
	}

	size_t GetNumCurrentKeysUp()
	{
		return currentUpKeys.size();
	}

	std::set<Keys> GetAllDownKeys()
	{
		return currentDownKeys;
	}

	std::set<Keys> GetAllUpKeys()
	{
		return currentUpKeys;
	}
}
