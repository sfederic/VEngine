#pragma once

#include <set>
#include <string>
#include "Win32Keys.h"

namespace Input
{
	extern bool mouseWheelUp;
	extern bool mouseWheelDown;

	extern bool blockInput;

	void InitKeyMap();
	void Reset();

	void PollInput();

	//Used for when Qt or ImGui might take away input WndProc focus from the program with a QtDialog for example,
	//meaning that hotkeys to access such dialogs will be stuck as held as held keys aren't cleared every frame.
	void ResetHeldKeys();

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
