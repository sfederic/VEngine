#pragma once
#include "Actor.h"
#include "ActorSystem.h"
#include <string>

struct MeshComponent;

struct MeshActor : Actor
{
	ACTOR_SYSTEM(MeshActor)

	MeshComponent* mesh = nullptr;

	inline static std::string spawnMeshFilename;

	MeshActor();
	virtual void Create() override;
	virtual Properties GetProps() override;
};
