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
		else if (Input::GetKeyUp(Keys::X))
		{
			Redo();
		}
	}
}

void CommandSystem::Add(ICommand* command)
{
	if (!commands.empty())
	{
		for (int i = commands.size() - 1; i > commandIndex; i--)
		{
			commands.pop_back();
		}
	}

	command->Execute();

	commands.push_back(command);

	commandIndex = commands.size() - 1;
}

void CommandSystem::Undo()
{
	if (commandIndex > 0)
	{
		commandIndex--;
	}

	commands[commandIndex]->Undo();
}

void CommandSystem::Redo()
{
	if (commandIndex < commands.size() - 1)
	{
		commandIndex++;
		commands[commandIndex]->Execute();
	}
}
