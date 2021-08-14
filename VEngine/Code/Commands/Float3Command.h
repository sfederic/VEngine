#pragma once
#include <DirectXMath.h>
#include "ICommand.h"

using namespace DirectX;

struct Float3Command : ICommand
{
	XMFLOAT3 oldValue;
	XMFLOAT3 newValue;
	XMFLOAT3* value;

	Float3Command(XMFLOAT3 newValue_, XMFLOAT3* value_)
	{
		value = value_;
		newValue = newValue_;
		oldValue = XMFLOAT3(0.f, 0.f, 0.f);
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
