#pragma once

#include <DirectXMath.h>

struct Transform
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX local;

	DirectX::XMFLOAT4 rotation; //Quaternion
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;

	Transform();
	DirectX::XMMATRIX GetAffine();
	DirectX::XMMATRIX GetAffineRotationOrigin(DirectX::XMVECTOR rotOrigin);
	void Decompose(DirectX::XMMATRIX m);
};
