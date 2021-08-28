#include "CommandSystem.h"
#include "ICommand.h"
#include "Input.h"

CommandSystem commandSystem;

void CommandSystem::Tick()
{
	if (Input::GetAsyncKey(Keys::Ctrl))
	{
		if (Input::GetKeyUp(Keys::Z))
		{
			Undo();
		}
	}
}

void CommandSystem::AddCommand(ICommand* command)
{
	command->Execute();
	commands.push_back(command);
	currentCommandIndex = commands.size() - 1;
}

void CommandSystem::Undo()
{
	if (currentCommandIndex <= 0)
	{
		if (commands.size() > 0)
		{
			commands.front()->Undo();
		}

		return; //Reached start of list
	}

	commands[currentCommandIndex]->Undo();
	currentCommandIndex--;
}

void CommandSystem::Redo()
{
	if (currentCommandIndex >= commands.size() - 1)
	{
		commands.back()->Execute();
		return; //Reached end of list
	}

	currentCommandIndex++;
	commands[currentCommandIndex]->Execute();
}
