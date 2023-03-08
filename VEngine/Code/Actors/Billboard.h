#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class MeshComponent;

class Billboard : public Actor
{
public:
	ACTOR_SYSTEM(Billboard);

	Billboard();
	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	MeshComponent* mesh = nullptr;
};
