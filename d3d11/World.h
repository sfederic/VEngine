#pragma once

#include <vector>
#include "Actor.h"

class World
{
public:

	void CleaupAllActors();

	std::vector<ActorSystem> actorSystems;
	char name[64];
};

extern World currentWorld;

World* GetWorld();
