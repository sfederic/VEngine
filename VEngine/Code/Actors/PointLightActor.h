#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct PointLightComponent;

struct PointLightActor : Actor
{
	ACTOR_SYSTEM(PointLightActor)

	PointLightComponent* pointLight;

	PointLightActor();
	virtual Properties GetProps() override;
};
