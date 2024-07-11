#include "vpch.h"
#include "Input.h"
#include <GameInput.h>
#include "Core/Debug.h"

IGameInput* gGameInput;
IGameInputDevice* gGamepad;

std::vector<GameInputKeyState> gPreviousFrameKeyState;

IGameInputReading* keyboardInputReading;
IGameInputReading* mouseInputReading;

std::unordered_set<Keys> currentHeldKeys;
std::unordered_set<Keys> currentDownKeys;
std::unordered_set<Keys> currentUpKeys;

std::unordered_set<Keys> pressedSystemKeys;
std::unordered_set<Keys> releasedSystemKeys;
std::unordered_set<Keys> heldSystemKeys;

std::multimap<std::string, Keys> keyMap;

bool mouseWheelUp;
bool mouseWheelDown;
bool mouseLeftUp;
bool mouseLeftDown;
bool mouseRightUp;
bool mouseRightDown;
bool mouseMiddleUp;
bool mouseMiddleDown;

bool blockInput;

void PollKeyboardInput();
void InitKeyMap();

namespace Input
{
	void PollInput()
	{
		PollKeyboardInput();
	}

	void Init()
	{
		HR(GameInputCreate(&gGameInput));
		InitKeyMap();
	}

	void Reset()
	{
		currentUpKeys.clear();
		currentDownKeys.clear();

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
		currentHeldKeys.clear();
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

	bool GetKeyDown(std::string keyMapping)
	{
		auto mappingIt = keyMap.find(keyMapping);
		if (mappingIt != keyMap.end())
		{
			if (currentDownKeys.contains(mappingIt->second))
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
			if (currentUpKeys.contains(mappingIt->second))
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
			if (currentHeldKeys.contains(mappingIt->second))
			{
				return true;
			}
		}
		return false;
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

	std::unordered_set<Keys> GetAllDownKeys()
	{
		return currentDownKeys;
	}

	std::unordered_set<Keys> GetAllUpKeys()
	{
		return currentUpKeys;
	}
}

void PollKeyboardInput()
{
	const auto ScanCodeToVirtualKey = [](UINT scanCode) -> Keys
		{
			const UINT key = MapVirtualKeyA(scanCode, MAPVK_VSC_TO_VK);
			return (Keys)key;
		};

	HR(gGameInput->GetCurrentReading(GameInputKindKeyboard, gGamepad, &keyboardInputReading));

	const uint32_t keyCount = keyboardInputReading->GetKeyCount();
	std::vector<GameInputKeyState> keyStates(keyCount);
	keyboardInputReading->GetKeyState(keyCount, keyStates.data());

	for (const auto& keyState : keyStates)
	{
		const Keys key = ScanCodeToVirtualKey(keyState.scanCode);
		Input::SetKeyDown(key);
	}

	//Set key up if doesn't exist on current frame.
	for (const auto& keyState : gPreviousFrameKeyState)
	{
		const auto key = ScanCodeToVirtualKey(keyState.scanCode);
		if (!Input::GetKeyDown(key))
		{
			Input::SetKeyUp(key);
		}
	}

	gPreviousFrameKeyState = keyStates;
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
	keyMap.emplace("Interact", Keys::Down);
	keyMap.emplace("OpenJournal", Keys::J);
}
