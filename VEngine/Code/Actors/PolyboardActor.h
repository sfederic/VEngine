#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct Polyboard;

//@Todo: Keeping this here just for visual testing. In the end, try to consolidate Polyboards and
//Particle Emitters into ParticleSystem the same way for example UE4 lets you spawn Ribbon/Beam/Point
//particles from one ParticleComponent

struct PolyboardActor : Actor
{
	ACTOR_SYSTEM(PolyboardActor);

	Polyboard* polyboard = nullptr;

	PolyboardActor();
	virtual Properties GetProps() override;
};
