#include <DirectXMath.h>

export module Render.ShaderData.ShaderMeshData;

export struct ShaderMeshData
{
	DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0.f, 0.f, 0.f);

private:
	float padding;
};
