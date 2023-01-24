#pragma once

#include <DirectXMath.h>

using namespace DirectX;

struct ParticleData
{
	XMFLOAT3 direction = XMFLOAT3(0.f, 0.f, 0.f);

	//The Float2's here are acting as a range [x = min, y = max]
	XMFLOAT2 moveSpeed = XMFLOAT2(1.f, 1.f);
	XMFLOAT2 lifetime = XMFLOAT2(1.f, 1.f);
	XMFLOAT2 spawnRate = XMFLOAT2(0.3f, 0.3f);
	XMFLOAT2 spawnRadius = XMFLOAT2(0.f, 1.f);
	XMFLOAT2 rotation = XMFLOAT2(0.f, 0.f);
	XMFLOAT2 rotateSpeed = XMFLOAT2(0.f, 0.f);
};
