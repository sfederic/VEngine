#pragma once

#include <DirectXMath.h>

using namespace DirectX;

struct PointLight
{
	XMFLOAT4 ambient;
	XMFLOAT3 pos;
	float range;
};

