#pragma once
#include "Actors/Actor.h"
#include "Actors/ActorSystem.h"

struct ParticleEmitter;

//@Todo: there's something wrong with the rootcomponent transform in ParticleSystem. ImGuizmo isn't showing
//the transform gizmo at the actor's position in world, defaults to [0, 0, 0]

struct ParticleSystem : Actor
{
	ACTOR_SYSTEM(ParticleSystem)

	std::vector<ParticleEmitter*> emitters;
	
	ParticleSystem();
	void AddEmitter(ParticleEmitter* emitter);
	virtual Properties GetProps() override;
};
