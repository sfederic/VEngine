#pragma once
#include "Actor.h"
#include "ActorSystem.h"
#include <string>

struct MeshComponent;

struct MeshActor : Actor
{
	ACTOR_SYSTEM(MeshActor)

	MeshComponent* mesh = nullptr;

	std::string meshFilename;

	MeshActor(std::string meshFilename_ = "");
};
