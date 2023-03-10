#pragma once

#include "Core/Transform.h"

struct ParticleData;

struct Particle
{
	Transform transform;
	DirectX::XMFLOAT3 direction = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	float lifetime = 0.f;
	float angle = 0.f;
	float yaw = 0.f;
	float pitch = 0.f;
	float roll = 0.f;
	float rotateSpeed = 0.f;
	float moveSpeed = 0.f;

	void AddVelocity(float deltaTime)
	{
		transform.position.x += (direction.x * moveSpeed) * deltaTime;
		transform.position.y += (direction.y * moveSpeed) * deltaTime;
		transform.position.z += (direction.z * moveSpeed) * deltaTime;
	}

	void SetParticleRangeData(ParticleData& particleData);
};
