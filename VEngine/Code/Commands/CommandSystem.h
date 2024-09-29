#pragma once

#include <vector>
#include <memory>
#include "Commands/Command.h"

class Actor;

class CommandSystem
{
public:
	static auto& Get()
	{
		static CommandSystem commandSystem;
		return commandSystem;
	}

	template <typename T>
	void AddCommand(Property& prop)
	{
		commands.emplace_back(std::make_unique<Command<T>>(prop));
	}

	void AddDeleteActorCommand(Actor* actorToDelete);

	void Tick();
	void Reset();

private:
	void Undo();
	void Redo();

	std::vector<std::unique_ptr<ICommand>> commands;
	size_t commandIndex = 0;
};
