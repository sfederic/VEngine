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

	//TODO: In Maya and shit, they have a Repeat function (Ctrl+Y) that repeats in the top command.
	//Might be worth looking into.
	//void Repeat();
};

extern CommandSystem commandSystem;
