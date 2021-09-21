#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct BoxTriggerComponent;

struct EntranceTrigger : Actor
{
	ACTOR_SYSTEM(EntranceTrigger)

	BoxTriggerComponent* trigger;

	EntranceTrigger();
	virtual Properties GetProps() override;
};
