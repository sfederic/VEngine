#include "World.h"
#include "Actor.h"

World currentWorld;

//Returns the current active world
World* GetWorld()
{
	return &currentWorld;
}

void World::CleaupAllActors()
{
	for (int systemIndex = 0; systemIndex < actorSystems.size(); systemIndex++)
	{
		//TODO: no good. need to start allocing on heap for actors for cleanup here (maybe std::ptrs). or figrue out a way statically
		actorSystems[systemIndex].actors.clear();
	}

	actorSystems.clear();
}
