#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct InstanceMeshComponent;

struct NormalActor : Actor
{
	ACTOR_SYSTEM(NormalActor)

	InstanceMeshComponent* mesh1;

	NormalActor();
};
