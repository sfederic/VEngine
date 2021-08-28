#pragma once
#include <vector>

struct ICommand;

struct CommandSystem
{
	std::vector<ICommand*> commands;
	uint32_t commandIndex = 0;

	void Tick();
	void Add(ICommand* command);
	void Undo();
	void Redo();
};

extern CommandSystem commandSystem;
