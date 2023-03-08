#include "vpch.h"
#include "CommandSystem.h"
#include "ICommand.h"
#include "Core/Input.h"

std::vector<ICommand*> CommandSystem::commands;
uint32_t CommandSystem::commandIndex = 0;

void Undo();
void Redo();

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

void Undo()
{
	CommandSystem::commands[CommandSystem::commandIndex]->Undo();

	if (CommandSystem::commandIndex > 0)
	{
		CommandSystem::commandIndex--;
	}
}

void Redo()
{
	if (CommandSystem::commandIndex < CommandSystem::commands.size() - 1)
	{
		CommandSystem::commandIndex++;
		CommandSystem::commands[CommandSystem::commandIndex]->Execute();
	}
}

void CommandSystem::Reset()
{
	commands.clear();
	commandIndex = 0;
}
