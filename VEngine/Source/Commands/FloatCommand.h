#pragma once

#include "ICommand.h"

class FloatCommand : public ICommand
{
public:
	FloatCommand(float newValue, float* value_);
	virtual void Execute() override;
	virtual void Undo() override;

private:
	float* value;
	float oldValue;
	float newValue;
};

