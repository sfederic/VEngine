export module Editor.Console;

import <map>;
import <string>;
import <functional>;

export namespace Console
{
	extern bool bConsoleActive;

	//Holds the functions to call that are paired to their console command text
	extern std::map<std::wstring, std::pair<std::function<void()>, std::string>> executeMap;

	void Init();
	void ConsoleInput();
	void Tick();

	//Needs a Tick() happening outside of D2D's Begin/EndDraw calls because swapchain resizes from lightprobe
	//bakes and whatever else will error in UISystem cleanup when it tries to Release() D2D buffers
	void InputTick();

	void ExecuteString();
};
