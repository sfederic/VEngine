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
	actorSystems.clear();
}
