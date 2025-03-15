export module Core.Input;

import Core.Win32Keys;
import <unordered_set>;
import <string>;

//Todo: Input has the differentiation of "system" and "gameplay" keys. System means it's coming from the OS' Win32 handling
//directly. There are calls to use "gameplay" keys in Input, but it's still using Win32 for now, previously was using
//GameInput, Microsoft's package. Keep an eye on the state of that package, or maybe use SDL2 or something for 
//controller input later down the line.

export namespace Input
{
	void Init();
	void Reset();

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
	bool GetSystemKeyHeld(Keys key);
	std::unordered_set<Keys> GetAllPressedSystemKeys();
	std::unordered_set<Keys> GetAllReleasedSystemKeys();

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
}
