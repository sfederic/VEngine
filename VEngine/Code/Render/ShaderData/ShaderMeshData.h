#pragma once

#include <DirectXMath.h>

struct ShaderMeshData
{
	DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0.f, 0.f, 0.f);

private:
	float padding;
};
