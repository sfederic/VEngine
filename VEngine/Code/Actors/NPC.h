#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct NPC : Actor
{
	ACTOR_SYSTEM(NPC)

	virtual Properties GetProps() override;
};
