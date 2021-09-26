#pragma once

//Incremental linking and debug fast link reference for VS project settings
//REF: https://devblogs.microsoft.com/cppblog/faster-c-iteration-builds/

struct Engine
{
	void Init(int argc, char* argv[]);
	void TickSystems(double deltaTime);
	void ResetSystems();
	void MainLoop();
	void Render(double deltaTime);
	void Cleanup();
};

extern Engine engine;
