#pragma once
#include <vector>
#include "Transform.h"

struct Actor;

struct IActorSystem
{
	virtual void GetActors(std::vector<Actor*>& outActors) = 0;
	virtual Actor* SpawnActor(Transform transform) = 0;
};
