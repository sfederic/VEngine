#pragma once

#include <DirectXMath.h>

struct InstanceData
{
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT4 colour = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
};

struct LightProbeInstanceData
{
	XMFLOAT4 SH[9]{}; //Spherical Harmonics
	DirectX::XMMATRIX modelMatrix = DirectX::XMMatrixIdentity();
	XMFLOAT3 position = XMFLOAT3(0.f, 0.f, 0.f);
	int index = 0;
};
