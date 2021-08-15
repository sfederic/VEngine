#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct NormalActor : Actor
{
	ACTOR_SYSTEM(NormalActor)

	struct MeshComponent* mesh;

	NormalActor();
};
