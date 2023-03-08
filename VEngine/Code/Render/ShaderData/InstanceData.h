#pragma once

#include <DirectXMath.h>

struct InstanceData
{
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT4 colour = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
};
