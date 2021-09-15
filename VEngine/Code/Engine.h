#pragma once

//
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
