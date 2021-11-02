#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct Billboard : Actor
{
	ACTOR_SYSTEM(Billboard)

	struct MeshComponent* mesh = nullptr;

	XMVECTOR velocity;
	float lifeTime = 0.f;

	Billboard();
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
};
