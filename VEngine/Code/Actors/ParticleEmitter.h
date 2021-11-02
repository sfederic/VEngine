#pragma once
#include "Actor.h"
#include "ActorSystem.h"
#include <vector>

struct ParticleEmitter : Actor
{
	ACTOR_SYSTEM(ParticleEmitter)

	XMFLOAT3 particleDirectionMin;
	XMFLOAT3 particleDirectionMax;

	float spawnTimer = 1.f;
	float spawnRate = 1.f;
	float speed = 1.f;

	ParticleEmitter();
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
};
