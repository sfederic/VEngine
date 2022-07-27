#pragma once
#include <string>
#include <map>
#include <functional>

namespace Console
{
	extern bool bConsoleActive;

	void Init();
	void ConsoleInput();
	void Tick();

	//Needs a Tick() happening outside of D2D's Begin/EndDraw calls because swapchain resizes from lightprobe
	//bakes and whatever else will error in UISystem cleanup when it tries to Release() D2D buffers
	void InputTick();

	void ExecuteString();
};
