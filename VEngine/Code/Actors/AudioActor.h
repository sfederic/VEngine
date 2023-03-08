#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class AudioComponent;

class AudioActor : public Actor
{
public:
	ACTOR_SYSTEM(AudioActor);

	AudioActor();
	Properties GetProps() override;

private:
	AudioComponent* audioComponent = nullptr;
};
