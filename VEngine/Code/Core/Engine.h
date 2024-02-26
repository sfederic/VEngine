#pragma once

namespace Engine
{
	void Init(int argc, char* argv[]);
	void TickSystems(float deltaTime);
	void MainLoop();
	void Render(float deltaTime);
	void Cleanup();
};
