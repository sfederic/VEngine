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
	void ExecuteString();
};

extern Console console;
