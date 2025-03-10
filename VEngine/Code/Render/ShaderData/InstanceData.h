#pragma once

#include <DirectXMath.h>

struct InstanceData
{
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT4 colour = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
};

struct LightProbeInstanceData
{
	DirectX::XMFLOAT4 SH[9]{}; //Spherical Harmonics
	DirectX::XMMATRIX modelMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	int index = 0;
};
