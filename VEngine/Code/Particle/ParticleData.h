#pragma once

import <DirectXMath.h>;
import Core.Properties;

using namespace DirectX;

struct ParticleData
{
	XMFLOAT3 minDirection = XMFLOAT3(0.f, 1.f, 0.f);
	XMFLOAT3 maxDirection = XMFLOAT3(0.f, 1.f, 0.f);

	//The Float2's here are acting as a range [x = min, y = max]
	XMFLOAT2 moveSpeed = XMFLOAT2(1.f, 1.f);
	XMFLOAT2 lifetime = XMFLOAT2(1.f, 1.f);
	XMFLOAT2 spawnRate = XMFLOAT2(0.f, 0.5f);
	XMFLOAT2 spawnRadius = XMFLOAT2(0.f, 1.f);
	XMFLOAT2 rotation = XMFLOAT2(0.f, 0.f);
	XMFLOAT2 rotateSpeed = XMFLOAT2(0.f, 0.f);

	Properties GetProps()
	{
		Properties props("ParticleData");
		props.Add("P_MinDirection", &minDirection);
		props.Add("P_MaxDirection", &maxDirection);
		props.Add("P_MoveSpeed", &moveSpeed);
		props.Add("P_Lifetime", &lifetime);
		props.Add("P_SpawnRate", &spawnRate);
		props.Add("P_SpawnRadius", &spawnRadius);
		props.Add("P_Rotation", &rotation);
		props.Add("P_RotateSpeed", &rotateSpeed);
		return props;
	}
};
