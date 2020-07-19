#pragma once

#include <vector>

//Class acts as the current game world, including all actors 
class World
{
public:

	std::vector<class ActorSystem*> actorSystems;
	char name[64];
};

static World g_CurrentWorld;

World* GetWorld();
