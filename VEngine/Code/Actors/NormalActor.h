#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct MeshComponent;

struct NormalActor : Actor
{
	ACTOR_SYSTEM(NormalActor)

	MeshComponent* mesh1;
	MeshComponent* mesh2;

	NormalActor();
};
