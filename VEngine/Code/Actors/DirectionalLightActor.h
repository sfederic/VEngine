#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct DirectionalLightComponent;

struct DirectionalLightActor : Actor
{
	ACTOR_SYSTEM(DirectionalLightActor)

	DirectionalLightComponent* directionalLight;

	DirectionalLightActor();
	virtual Properties GetProps() override;
};
