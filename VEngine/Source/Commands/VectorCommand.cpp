#include "VectorCommand.h"

VectorCommand::VectorCommand(XMFLOAT3 newValue_, XMFLOAT3* value_)
{
	value = value_;
	newValue = newValue_;
	oldValue = XMFLOAT3(0.f, 0.f, 0.f);
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
