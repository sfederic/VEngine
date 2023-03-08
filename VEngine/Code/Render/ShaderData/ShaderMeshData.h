#pragma once

#include <DirectXMath.h>

struct ShaderMeshData
{
	DirectX::XMFLOAT4 SH[9];
	DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	float pad;
};
