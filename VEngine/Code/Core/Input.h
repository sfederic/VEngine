#pragma once

#include <set>
#include <string>
#include "Win32Keys.h"

//Input has the deferentiation of "system" and "gameplay" keys. System means it's coming from the OS' Win32 handling
//directly, and Gameplay meaning from GameInput's API, primarily used during gameplay. System key input would be 
//used more for text editing and the like.

namespace Input
{
	void Init();
	void Reset();

	void PollInput();

	//Used for when Qt or ImGui might take away input WndProc focus from the program with a QtDialog for example,
	//meaning that hotkeys to access such dialogs will be stuck as held as held keys aren't cleared every frame.
	void ResetHeldKeys();

	void SetBlockInput(bool value);

	void SetMouseWheelUp();
	void SetMouseWheelDown();
	bool IsMouseWheelUp();
	bool IsMouseWheelDown();

	void SetSystemKeyDown(Keys key);
	void SetSystemKeyUp(Keys key);
	bool GetSystemKeyDown(Keys key);
	bool GetSystemKeyUp(Keys key);
	std::set<Keys> GetAllPressedSystemKeys();
	std::set<Keys> GetAllReleasedSystemKeys();

	void SetKeyDown(Keys key);
	void SetKeyUp(Keys key);

	bool GetKeyDown(std::string keyMapping);
	bool GetKeyUp(std::string keyMapping);
	bool GetKeyHeld(std::string keyMapping);

	bool GetKeyDown(Keys key);
	bool GetKeyUp(Keys key);
	bool GetKeyHeld(Keys key);

	bool GetAnyKeyDown();
	bool GetAnyKeyUp();

	void SetLeftMouseUp();
	void SetLeftMouseDown();
	void SetRightMouseUp();
	void SetRightMouseDown();
	void SetMiddleMouseUp();
	void SetMiddleMouseDown();

	bool GetMouseLeftUp();
	bool GetMouseRightUp();
	bool GetMouseLeftDown();
	bool GetMouseRightDown();
	bool GetMouseMiddleUp();
	bool GetMouseMiddleDown();

	size_t GetNumCurrentKeysDown();
	size_t GetNumCurrentKeysUp();

	std::set<Keys> GetAllDownKeys();
	std::set<Keys> GetAllUpKeys();
}
