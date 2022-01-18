#pragma once
#include "Transform.h"

struct Particle
{
	Transform transform;
	float lifetime = 0.f;
	float angle = 0.f;
	float rotateSpeed = 0.f;
	float moveSpeed = 0.f;

	inline void AddVelocity(XMFLOAT3& direction, float deltaTime)
	{
		transform.position.x += (direction.x * moveSpeed) * deltaTime;
		transform.position.y += (direction.y * moveSpeed) * deltaTime;
		transform.position.z += (direction.z * moveSpeed) * deltaTime;
	}
};
