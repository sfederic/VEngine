#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"

struct BoxTriggerComponent;

//Trigger for player to interact with to memorise physical objects in-world for use later.
struct MemoryTrigger : Actor
{
	ACTOR_SYSTEM(MemoryTrigger);

	BoxTriggerComponent* trigger = nullptr;

	MemoryTrigger();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
};
