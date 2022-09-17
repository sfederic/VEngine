#include "vpch.h"
#include "CommandSystem.h"
#include "ICommand.h"
#include "Input.h"

CommandSystem commandSystem;

void CommandSystem::Tick()
{
	if (Input::GetKeyHeld(Keys::Ctrl))
	{
		if (Input::GetKeyDown(Keys::Z))
		{
			Undo();
		}
		else if (Input::GetKeyDown(Keys::Y))
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
	commands[commandIndex]->Undo();

	if (commandIndex > 0)
	{
		commandIndex--;
	}
}

void CommandSystem::Redo()
{
	if (commandIndex < commands.size() - 1)
	{
		commandIndex++;
		commands[commandIndex]->Execute();
	}
}

void CommandSystem::Reset()
{
	commands.clear();
	commandIndex = 0;
}

void CommandSystem::WindToCommand(uint32_t windCommandIndex)
{
	if (windCommandIndex < commandIndex)
	{
		for (int i = commandIndex; i > windCommandIndex; i--)
		{
			commands[i]->Undo();
		}

		commandIndex = windCommandIndex;
		commands[commandIndex]->Undo();
	}
	else if (windCommandIndex > commandIndex)
	{
		for (int i = commandIndex; i < windCommandIndex; i++)
		{
			commands[i]->Execute();
		}

		commandIndex = windCommandIndex;
		commands[commandIndex]->Execute();
	}
	else if (windCommandIndex == commandIndex)
	{
		commands[windCommandIndex]->Execute();
	}	
	else if (windCommandIndex == 0)
	{
		commands[windCommandIndex]->Undo();
	}
}
