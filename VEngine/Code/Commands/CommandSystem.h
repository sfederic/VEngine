#pragma once
#include <vector>
#include "Command.h"

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

	//TODO: In Maya and shit, they have a Repeat function (Ctrl+Y) that repeats in the top command.
	//Might be worth looking into.
	//void Repeat();
};

extern CommandSystem commandSystem;
