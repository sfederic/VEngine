#include <DirectXMath.h>

export module Render.LightData;

export enum class LightType : int
{
	Directional = 0,
	Point = 1,
	Spot = 2
};

export struct LightData
{
	DirectX::XMFLOAT4 position = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	DirectX::XMFLOAT4 direction = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	DirectX::XMFLOAT4 colour = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	float spotAngle = 70.f;
	float intensity = 25.0f;
	float range = 100.f;
	int lightType = 0;
	int enabled = 1;

private:
	int padding[3];
};
