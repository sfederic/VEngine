#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class MeshComponent;

//Todo: not sure if I want this around. Might be better to do a VEnum as a property, linking
//to the PhysicsActorType enum. See how it goes.
class SpherePhysicsActor : public Actor
{
public:
	ACTOR_SYSTEM(SpherePhysicsActor);

	SpherePhysicsActor();
	void Create() override;
	Properties GetProps() override;

private:
	MeshComponent* mesh = nullptr;
};
