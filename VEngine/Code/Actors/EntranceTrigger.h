#pragma once
#include "Actor.h"

struct BoxTriggerComponent;

struct EntranceTrigger : Actor
{
	BoxTriggerComponent* trigger;

	EntranceTrigger();
	virtual Properties GetProps() override;
};
