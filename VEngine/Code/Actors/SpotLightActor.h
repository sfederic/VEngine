#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class SpotLightComponent;

class SpotLightActor : public Actor
{
public:
	ACTOR_SYSTEM(SpotLightActor);

	SpotLightActor();
	Properties GetProps() override;

private:
	SpotLightComponent* spotLight = nullptr;
};
