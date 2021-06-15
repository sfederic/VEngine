#include "FloatCommand.h"

FloatCommand::FloatCommand(float newValue_, float* value_)
{
	value = value_;
	newValue = newValue_;
}

void FloatCommand::Execute()
{
	oldValue = *value;
	*value = newValue;
}

void FloatCommand::Undo()
{
	*value = oldValue;
}
