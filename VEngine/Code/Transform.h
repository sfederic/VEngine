#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct Transform
{
	XMMATRIX world;
	XMMATRIX local;

	XMFLOAT4 rotation; //Quaternion
	XMFLOAT3 position;
	XMFLOAT3 scale;

	Transform();
	XMMATRIX GetAffine();
	XMMATRIX GetAffineRotationOrigin(XMVECTOR rotOrigin);
	void Decompose(XMMATRIX m);
};
