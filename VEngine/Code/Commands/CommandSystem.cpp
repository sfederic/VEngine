#include "vpch.h"
#include "CommandSystem.h"
#include "ICommand.h"
#include "Core/Input.h"
#include "Editor/Editor.h"

std::vector<ICommand*> CommandSystem::commands;
uint32_t CommandSystem::commandIndex = 0;

void Undo();
void Redo();

void CommandSystem::Add(ICommand* value)
{
	commands.push_back(value);
	commandIndex = commands.size() - 1;
}

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
	if (CommandSystem::commandIndex > 0)
	{
		CommandSystem::commandIndex--;
		CommandSystem::commands[CommandSystem::commandIndex]->Execute();
	}
	else
	{
		CommandSystem::commands[0]->Execute();
	}

	editor->ResetPropertyWidgetValues();
}

void Redo()
{
	if (CommandSystem::commandIndex < CommandSystem::commands.size() - 1)
	{
		CommandSystem::commandIndex++;
		CommandSystem::commands[CommandSystem::commandIndex]->Execute();
	}
	else
	{
		CommandSystem::commands[CommandSystem::commands.size() - 1]->Execute();
	}

	editor->ResetPropertyWidgetValues();
}

void CommandSystem::Reset()
{
	commands.clear();
	commandIndex = 0;
}
