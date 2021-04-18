#pragma once

#include "Actor.h"

//Lot of different ways to do particles (Geom shaders, stream out, instancing, dynamic vertex buffer).
//This iteration is going with instancing a single plane with each particle being a billboard.

class Particle : public Actor
{
public:

};

//TODO: figure out if you need a system that manages every particle or whether it's
//light enough to throw it in with the actors.
class ParticleSystem : public ActorSystem
{
public:
	ParticleSystem();

	virtual void Tick();

	Transform transform;
};
