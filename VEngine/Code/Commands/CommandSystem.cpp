#include "vpch.h"
#include "CommandSystem.h"
#include "ICommand.h"
#include "Core/Input.h"
#include "Editor/Editor.h"

std::vector<ICommand*> commands;
uint32_t commandIndex = 0;

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
	if (commandIndex > 0)
	{
		commandIndex--;
		commands[commandIndex]->Execute();
		delete commands.back();
		commands.pop_back();
	}
	else
	{
		commands[0]->Execute();
	}

	editor->ResetPropertyWidgetValues();
}

void Redo()
{
	if (commandIndex < commands.size() - 1)
	{
		commandIndex++;
		commands[commandIndex]->Execute();
	}
	else
	{
		commands[commands.size() - 1]->Execute();
	}

	editor->ResetPropertyWidgetValues();
}

void CommandSystem::Reset()
{
	for (auto command : commands)
	{
		delete command;
	}
	commands.clear();
	commandIndex = 0;
}
