#pragma once
#include "Actor.h"
#include "ActorSystem.h"
#include <string>

struct BoxTriggerComponent;

struct EntranceTrigger : Actor
{
	ACTOR_SYSTEM(EntranceTrigger)

	BoxTriggerComponent* trigger = nullptr;

	std::string levelToMoveTo;

	EntranceTrigger();
	virtual void Start() override;
	virtual void Tick(double deltaTime) override;
	virtual Properties GetProps() override;
};
