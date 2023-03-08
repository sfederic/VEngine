#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class DirectionalLightComponent;

class DirectionalLightActor : public Actor
{
public:
	ACTOR_SYSTEM(DirectionalLightActor)

	DirectionalLightActor();
	Properties GetProps() override;

private:
	DirectionalLightComponent* directionalLight = nullptr;
};
