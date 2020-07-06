#pragma once

#include <vector>

struct ConsoleViewItem
{
	wchar_t text[128];
};

extern std::vector<ConsoleViewItem> viewItems;


//TODO: need to work console away from DebugMenu features
//i.e. static and dynamic variable views are no good (or at least open the appropriate DebugMenu windows)
//Keep console on functionality (open levels, off/on switches?)
class Console
{
public: 
	static void ConsoleInput();
	static void Tick(class UIContext* ui, class DXUtil* dx);
	static void ExecuteString(class DXUtil* dx);

	static void DrawViewItems(UIContext* ui);

};

