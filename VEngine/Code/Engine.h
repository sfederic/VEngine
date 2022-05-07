#pragma once

struct Engine
{
	void Init(int argc, char* argv[]);
	void TickSystems(float deltaTime);
	void ResetSystems();
	void MainLoop();
	void Render(float deltaTime);
	void Cleanup();
};

extern Engine engine;
