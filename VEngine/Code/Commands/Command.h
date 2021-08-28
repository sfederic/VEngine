#pragma once
#include "ICommand.h"

template <typename T>
struct Command : ICommand
{
	T oldValue;
	T newValue;
	T* value;

	Command(T newValue_, T* value_)
	{
		value = value_;
		newValue = newValue_;
	}

	void Execute() override
	{
		oldValue = *value;
		*value = newValue;
	}

	void Undo() override
	{
		*value = oldValue;
	}
};
