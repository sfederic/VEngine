#include "vpch.h"
#include "CommandSystem.h"
#include "Core/Input.h"
#include "Editor/Editor.h"

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

void CommandSystem::Undo()
{
	if (commands.size() == 0)
	{
		return;
	}

	if (commandIndex > 0)
	{
		commandIndex--;
		commands[commandIndex]->Execute();
		commands.pop_back();
	}
	else
	{
		commands[0]->Execute();
	}

	editor->ResetPropertyWidgetValues();
}

void CommandSystem::Redo()
{
	if (commands.size() == 0)
	{
		return;
	}

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
	commands.clear();
	commandIndex = 0;
}
