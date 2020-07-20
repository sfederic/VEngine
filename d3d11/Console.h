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
	void Tick(class World* world);
	void ExecuteString(class World* world);
	void DrawViewItems();

	std::vector<ConsoleViewItem> viewItems;
};

