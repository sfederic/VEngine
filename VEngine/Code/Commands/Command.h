#pragma once
#include "ICommand.h"

template <typename T>
struct Command : ICommand
{
	T oldValue;
	T newValue;
	T* value;

	//use this constructor to push new values into the command
	Command(T newValue_, T* value_)
	{
		value = value_;
		newValue = newValue_;
	}

	//Use this one when you just need the pointer value pushed in
	Command(T* value_)
	{
		value = value_;
		newValue = *value_;
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
