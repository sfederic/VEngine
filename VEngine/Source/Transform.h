#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class Transform
{
public:
	Transform();
	void Decompose(XMMATRIX m);

	XMFLOAT3 position;
	XMFLOAT3 oldPosition;

	XMFLOAT3 scale;
	XMFLOAT3 oldScale;

	XMFLOAT3 euler;

	XMFLOAT4 quatRotation;
	XMFLOAT4 oldQuatRotation;
};
