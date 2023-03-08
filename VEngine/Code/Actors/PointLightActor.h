#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class PointLightComponent;

class PointLightActor : public Actor
{
public:
	ACTOR_SYSTEM(PointLightActor)

	PointLightActor();
	Properties GetProps() override;

private:
	PointLightComponent* pointLight = nullptr;
};
