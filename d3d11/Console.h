#pragma once

#include <vector>

struct ConsoleViewItem
{
	wchar_t text[128];
};

extern std::vector<ConsoleViewItem> viewItems;

class Console
{
public: 
	static void ConsoleInput();
	static void Tick(class UIContext* ui, class DXUtil* dx);
	static void ExecuteString(class DXUtil* dx);

	static void DrawViewItems(UIContext* ui);

};

