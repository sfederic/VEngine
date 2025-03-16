import Commands.CommandSystem;
import Actors.Actor;
import Actors.IActorSystem;
import Core.Input;
import Core.Win32Keys;
import Editor.Editor;
import <vector>;
import <memory>;

void CommandSystem::AddDeleteActorCommand(Actor* actorToDelete)
{
	auto actorSystem = actorToDelete->GetActorSystem();
	commands.push_back(std::make_unique<UndoActorDeleteCommand>(actorSystem));
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
	}
	else
	{
		commands[0]->Execute();
	}

	Editor::Get().ResetPropertyWidgetValues();
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

	Editor::Get().ResetPropertyWidgetValues();
}

void CommandSystem::Reset()
{
	commands.clear();
	commandIndex = 0;
}
