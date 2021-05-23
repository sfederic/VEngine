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

	std::vector<ConsoleViewItem> viewItems;
	std::wstring consoleString;

	bool bConsoleActive = false;
};

extern Console gConsole;
