#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct BoxTriggerComponent;

//Used to open up a quick prompt/action when the player overlaps
struct InteractTrigger : Actor
{
	ACTOR_SYSTEM(InteractTrigger)

	BoxTriggerComponent* trigger = nullptr;

	InteractTrigger();
	virtual void Start() override;
	virtual void Tick(double deltaTime) override;
	virtual Properties GetProps() override;
};
