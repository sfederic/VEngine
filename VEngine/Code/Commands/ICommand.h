#pragma once
#include <string>

struct ICommand
{
	std::string name;

	virtual void Execute() = 0;
	virtual void Undo() = 0;
};
