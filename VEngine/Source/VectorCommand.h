#pragma once

#include "ICommand.h"
#include <DirectXMath.h>

using namespace DirectX;

class VectorCommand : public ICommand
{
public:
	VectorCommand(XMFLOAT3 newValue_, XMFLOAT3* value_);
	virtual void Execute() override;
	virtual void Undo() override;

private:
	XMFLOAT3 oldValue;
	XMFLOAT3 newValue;
	XMFLOAT3* value;
};
