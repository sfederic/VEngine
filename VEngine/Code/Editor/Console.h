#pragma once
#include <string>
#include <map>
#include <functional>

struct Console
{
	//Holds the functions to call that are paired to their console command text
	std::map<std::wstring, std::function<void()>> executeMap;

	std::wstring consoleString;
	bool bConsoleActive = false;

	Console();
	void ConsoleInput();
	void Tick();

	//Needs a Tick() happening outside of D2D's Begin/EndDraw calls because swapchain resizes from lightprobe
	//bakes and whatever else will error in UISystem cleanup when it tries to Release() D2D buffers
	void InputTick();

	void ExecuteString();
};

extern Console console;
