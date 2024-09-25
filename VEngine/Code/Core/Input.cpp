#include "vpch.h"
#include "Input.h"
#include "Core/Debug.h"

std::unordered_set<Keys> pressedSystemKeys;
std::unordered_set<Keys> releasedSystemKeys;
std::unordered_set<Keys> heldSystemKeys;

std::unordered_multimap<std::string, Keys> keyMap;

bool mouseWheelUp;
bool mouseWheelDown;
bool mouseLeftUp;
bool mouseLeftDown;
bool mouseRightUp;
bool mouseRightDown;
bool mouseMiddleUp;
bool mouseMiddleDown;

bool blockInput;

void InitKeyMap();

namespace Input
{
	void Init()
	{
		InitKeyMap();
	}

	void Reset()
	{
		pressedSystemKeys.clear();
		releasedSystemKeys.clear();

		mouseWheelUp = false;
		mouseWheelDown = false;
		mouseLeftUp = false;
		mouseRightUp = false;
		mouseMiddleUp = false;
	}

	void ResetHeldKeys()
	{
		heldSystemKeys.clear();
	}

	void SetBlockInput(bool value)
	{
		blockInput = value;
	}

	void SetMouseWheelUp()
	{
		mouseWheelUp = true;
	}

	void SetMouseWheelDown()
	{
		mouseWheelDown = true;
	}

	bool IsMouseWheelUp()
	{
		return mouseWheelUp;
	}

	bool IsMouseWheelDown()
	{
		return mouseWheelDown;
	}

	void SetSystemKeyDown(Keys key)
	{
		pressedSystemKeys.insert(key);
		heldSystemKeys.insert(key);
		releasedSystemKeys.erase(key);
	}

	void SetSystemKeyUp(Keys key)
	{
		releasedSystemKeys.insert(key);
		heldSystemKeys.erase(key);
		pressedSystemKeys.erase(key);
	}

	bool GetSystemKeyDown(Keys key)
	{
		if (blockInput) return false;
		return pressedSystemKeys.find(key) != pressedSystemKeys.end();
	}

	bool GetSystemKeyUp(Keys key)
	{
		if (blockInput) return false;
		return releasedSystemKeys.find(key) != releasedSystemKeys.end();
	}

	bool GetSystemKeyHeld(Keys key)
	{
		if (blockInput) return false;
		return heldSystemKeys.find(key) != heldSystemKeys.end();
	}

	std::unordered_set<Keys> GetAllPressedSystemKeys()
	{
		return pressedSystemKeys;
	}

	std::unordered_set<Keys> GetAllReleasedSystemKeys()
	{
		return releasedSystemKeys;
	}

	bool GetKeyDown(std::string keyMapping)
	{
		auto mappingIt = keyMap.find(keyMapping);
		if (mappingIt != keyMap.end())
		{
			if (pressedSystemKeys.contains(mappingIt->second))
			{
				return true;
			}
		}
		return false;
	}

	bool GetKeyUp(std::string keyMapping)
	{
		auto mappingIt = keyMap.find(keyMapping);
		if (mappingIt != keyMap.end())
		{
			if (releasedSystemKeys.contains(mappingIt->second))
			{
				return true;
			}
		}
		return false;
	}

	bool GetKeyHeld(std::string keyMapping)
	{
		auto mappingIt = keyMap.find(keyMapping);
		if (mappingIt != keyMap.end())
		{
			if (heldSystemKeys.contains(mappingIt->second))
			{
				return true;
			}
		}
		return false;
	}

	bool GetKeyDown(Keys key)
	{
		if (blockInput) return false;
		return pressedSystemKeys.find(key) != pressedSystemKeys.end();
	}

	bool GetKeyUp(Keys key)
	{
		if (blockInput) return false;
		return pressedSystemKeys.find(key) != pressedSystemKeys.end();
	}

	bool GetKeyHeld(Keys key)
	{
		if (blockInput) return false;
		return heldSystemKeys.find(key) != heldSystemKeys.end();
	}

	bool GetAnyKeyDown()
	{
		if (blockInput) return false;
		return pressedSystemKeys.size();
	}

	bool GetAnyKeyUp()
	{
		if (blockInput) return false;
		return releasedSystemKeys.size();
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
}

void InitKeyMap()
{
	keyMap.emplace("MoveForward", Keys::W);
	keyMap.emplace("MoveBack", Keys::S);
	keyMap.emplace("MoveLeft", Keys::A);
	keyMap.emplace("MoveRight", Keys::D);
	keyMap.emplace("RotateUp", Keys::Up);
	keyMap.emplace("RotateDown", Keys::Down);
	keyMap.emplace("RotateLeft", Keys::Left);
	keyMap.emplace("RotateRight", Keys::Right);
	keyMap.emplace("Link", Keys::Enter);
	keyMap.emplace("Unlink", Keys::BackSpace);
	keyMap.emplace("ToggleGrid", Keys::Space);
	keyMap.emplace("ToggleRecon", Keys::Q);
	keyMap.emplace("Interact", Keys::Down);
	keyMap.emplace("ReconSnapshot", Keys::Down);
	keyMap.emplace("OpenJournal", Keys::J);
}
