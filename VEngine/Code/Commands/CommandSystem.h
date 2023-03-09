#pragma once

#include <memory>
#include <vector>

struct ICommand;

//@Todo: CommandSystem isn't very good. Should be more generic and work with all sorts of properties.

//Currently used for undo/redo commands in the editor, but is generic enough with ICommand
//that it could be used during gameplay and for other systems.
namespace CommandSystem
{
	extern std::vector<ICommand*> commands;
	extern uint32_t commandIndex;

	void Add(ICommand* value);

	void Tick();
	void Reset();
};
