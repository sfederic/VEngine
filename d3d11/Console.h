#pragma once

#include <vector>

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
};

static Console console;