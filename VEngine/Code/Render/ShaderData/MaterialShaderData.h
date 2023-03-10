#pragma once

#include <DirectXMath.h>

struct MaterialShaderData
{
	DirectX::XMFLOAT4 ambient = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	DirectX::XMFLOAT4 emissive = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	DirectX::XMFLOAT4 diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	DirectX::XMFLOAT4 specular = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	DirectX::XMFLOAT2 uvOffset = DirectX::XMFLOAT2(0.f, 0.f);
	DirectX::XMFLOAT2 uvScale = DirectX::XMFLOAT2(1.f, 1.f);
	float uvRotation = 0.f;
	float smoothness = 0.f;
	float metallic = 0.f;
	int useTexture = true;
};
