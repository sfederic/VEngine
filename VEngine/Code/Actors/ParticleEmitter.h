#pragma once
#include "Actor.h"
#include "ActorSystem.h"
#include <vector>

struct Billboard;

struct ParticleEmitter : Actor
{
	ACTOR_SYSTEM(ParticleEmitter)

	float timer = 0.f;

	std::vector<Billboard*> bills;

	ParticleEmitter();
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
};
