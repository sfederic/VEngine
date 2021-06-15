#include "VectorCommand.h"

VectorCommand::VectorCommand(XMFLOAT3 newValue_, XMFLOAT3* value_)
{
	value = value_;
	newValue = newValue;
}

void VectorCommand::Execute()
{
	oldValue = *value;
	*value = newValue;
}

void VectorCommand::Undo()
{
	*value = oldValue;
}
