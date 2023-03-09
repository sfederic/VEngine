#pragma once

#include <vector>
#include <memory>
#include "Commands/ICommand.h"
#include "Commands/Command.h"
#include "Core/Property.h"

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
		commands.push_back(std::make_unique<Command<T>>(prop));
		commandIndex = commands.size() - 1;
	}
	
	void Tick();
	void Reset();

private:
	void Undo();
	void Redo();

	std::vector<std::unique_ptr<ICommand>> commands;
	uint32_t commandIndex = 0;
};
