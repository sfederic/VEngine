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
	static void Tick(class UISystem* ui, class RenderSystem* dx, class World* world);
	static void ExecuteString(class RenderSystem* dx, class World* world);

	static void DrawViewItems(UISystem* ui);

};

