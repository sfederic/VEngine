#pragma once
#include <vector>
#include <string>
#include "Transform.h"

struct Actor;

struct IActorSystem
{
	std::string name;

	virtual void GetActors(std::vector<Actor*>& outActors) = 0;
	virtual Actor* SpawnActor(Transform transform) = 0;
};
