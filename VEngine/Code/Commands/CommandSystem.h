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
	void Reset();

	//Wind back or forward to a selected command
	void WindToCommand(uint32_t windCommandIndex);
};

extern CommandSystem commandSystem;
