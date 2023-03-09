#pragma once

struct ICommand;

//@Todo: CommandSystem isn't very good. Should be more generic and work with all sorts of properties.

//Currently used for undo/redo commands in the editor, but is generic enough with ICommand
//that it could be used during gameplay and for other systems.
namespace CommandSystem
{
	void Add(ICommand* value);

	void Tick();
	void Reset();
};
