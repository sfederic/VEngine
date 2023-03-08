#pragma once

#include <memory>
#include <vector>
#include "Command.h"

struct ICommand;

//@Todo: CommandSystem isn't very good. Should be more generic and work with all sorts of properties.

//Currently used for undo/redo commands in the editor, but is generic enough with ICommand
//that it could be used during gameplay and for other systems.
namespace CommandSystem
{
	extern std::vector<ICommand*> commands;
	extern uint32_t commandIndex;

	template <typename T>
	void Add(T* value)
	{
		commands.push_back(new Command<T>(value));
		commandIndex = commands.size() - 1;
	}

	void Tick();
	void Reset();
};
