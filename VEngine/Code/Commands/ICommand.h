#pragma once

#include <string>

class ICommand
{
public:
	virtual ~ICommand() = default;
	virtual void Execute() = 0;
};
