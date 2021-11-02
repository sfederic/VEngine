#pragma once
#include "Actor.h"
#include "ActorSystem.h"
#include <vector>
#include "UI/SpriteBatcher.h"

struct ParticleEmitter : Actor
{
	ACTOR_SYSTEM(ParticleEmitter)

	std::vector<Sprite> particles;

	XMFLOAT3 particleDirectionMin;
	XMFLOAT3 particleDirectionMax;

	float spawnTimer = 1.f;
	float spawnRate = 1.f;
	float speed = 1.f;

	ParticleEmitter();
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
};
