#pragma once

#include <vector>
#include <string>

struct ConsoleViewItem
{
	wchar_t text[128];
};

class Console
{
public: 
	void ConsoleInput();
	void Tick();
	void ExecuteString();
	void DrawViewItems();

	std::wstring consoleString;

	std::vector<ConsoleViewItem> viewItems;
	//wchar_t consoleString[128];
	//int consoleStringIndex = 0;
	bool bConsoleActive = false;

};

extern Console gConsole;