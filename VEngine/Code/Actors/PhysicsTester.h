#pragma once

#include "Actor.h"
#include "ActorSystem.h"

//Profiler like actor to give some numbers between custom and physx scene queries.
class PhysicsTester : public Actor
{
public:
	ACTOR_SYSTEM(PhysicsTester);

	PhysicsTester();
	void Tick(float deltaTime) override;
	Properties GetProps() override { return __super::GetProps(); }
};
