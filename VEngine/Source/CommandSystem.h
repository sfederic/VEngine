#pragma once

#include "ICommand.h"
#include <vector>

//currently just for editor undo/redo, but I guess the pattern is generic enough that it 
//could be used in other spots.
class CommandSystem
{
public:
	void AddCommand(ICommand* command);
	void Undo();
	void Redo();

private:
	std::vector<ICommand*> commands;
	uint32_t currentCommandIndex = 0;
};

extern CommandSystem gCommandSystem;
