#pragma once
#include <string>

struct Console
{
	std::wstring consoleString;
	bool bConsoleActive = false;

	void ConsoleInput();
	void Tick();
	void ExecuteString();
};

extern Console console;
