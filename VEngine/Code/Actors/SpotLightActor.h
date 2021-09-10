#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct SpotLightActor : Actor
{
	ACTOR_SYSTEM(SpotLightActor)

	struct SpotLightComponent* spotLight;

	SpotLightActor();
	virtual Properties GetProps() override;
};
