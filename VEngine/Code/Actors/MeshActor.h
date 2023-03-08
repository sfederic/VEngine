#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class MeshComponent;

class MeshActor : public Actor
{
public:
	ACTOR_SYSTEM(MeshActor)

	MeshActor();
	void Create() override;
	Properties GetProps() override;

	inline static std::string spawnMeshFilename;

private:
	MeshComponent* mesh = nullptr;
};
