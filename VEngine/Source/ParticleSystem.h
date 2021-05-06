#pragma once

#include "Actor.h"

//Lot of different ways to do particles (Geom shaders, stream out, instancing, dynamic vertex buffer).
//This iteration is going with instancing a single plane with each particle being a billboard.

class Particle : public Actor
{
public:
	virtual Properties GetProperties() override;
};

class ParticleSystem : public ActorSystem
{
public:
	ParticleSystem();

	virtual void Tick(float deltaTime);

	Transform transform;
};
