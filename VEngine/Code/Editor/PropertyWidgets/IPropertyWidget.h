#pragma once

#include "Core/Property.h"
#include "Commands/CommandSystem.h"
#include "Commands/Command.h"

class IPropertyWidget
{
public:
	Property prop;
	virtual void ResetValue() = 0;

	template <typename T>
	void SetValue(Property& prop)
	{
		CommandSystem::Add(new Command<T>(prop));
	}
};
