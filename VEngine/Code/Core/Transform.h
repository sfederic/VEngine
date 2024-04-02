#pragma once

#include <DirectXMath.h>

struct Transform
{
	DirectX::XMMATRIX world;

	DirectX::XMFLOAT4 rotation; //Quaternion
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;

	Transform();
	DirectX::XMMATRIX GetAffine() const;
	DirectX::XMMATRIX GetAffineRotationOrigin(DirectX::XMVECTOR rotOrigin) const;
	void Decompose(DirectX::XMMATRIX m);
};
