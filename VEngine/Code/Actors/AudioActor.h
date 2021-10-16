#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct AudioComponent;

struct AudioActor : Actor
{
	ACTOR_SYSTEM(AudioActor)

	AudioComponent* audioComponent = nullptr;

	AudioActor();
	virtual Properties GetProps() override;
};
