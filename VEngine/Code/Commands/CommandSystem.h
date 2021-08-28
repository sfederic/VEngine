#pragma once
#include <vector>

struct ICommand;

struct CommandSystem
{
	std::vector<ICommand*> commands;
	uint32_t currentCommandIndex = 0;

	void Tick();
	void AddCommand(ICommand* command);
	void Undo();
	void Redo();
};

extern CommandSystem commandSystem;
