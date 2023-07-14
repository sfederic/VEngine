#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class PersistentAudioComponent;

class PersistentAudioActor : public Actor
{
public:
	ACTOR_SYSTEM(PersistentAudioActor);

	PersistentAudioActor();
	Properties GetProps() override;

private:
	PersistentAudioComponent* persistentAudio = nullptr;
};
