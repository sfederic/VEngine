#pragma once
#include "Actors/Actor.h"
#include "Actors/ActorSystem.h"

struct ParticleEmitter;

struct ParticleSystem : Actor
{
	ACTOR_SYSTEM(ParticleSystem)

	std::vector<ParticleEmitter*> emitters;
	
	ParticleSystem();
	void AddEmitter(ParticleEmitter* emitter);
	virtual Properties GetProps() override;
};
