#pragma once

#include <vector>

World* GetWorld();

class World
{
public:

	std::vector<class ActorSystem*> actorSystems;
	char name[64];
};

static World g_CurrentWorld;
