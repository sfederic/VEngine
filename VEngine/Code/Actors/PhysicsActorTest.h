#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class PhysicsActorTest : public Actor
{
public:
	ACTOR_SYSTEM(PhysicsActorTest);

	PhysicsActorTest();
	Properties GetProps() override;

private:
	class MeshComponent* mesh = nullptr;
};
