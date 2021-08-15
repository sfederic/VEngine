#include "World.h"
#include "Actors/NormalActor.h"

World world;

void World::Start()
{
	activeActorSystems.push_back(&NormalActor::system);
}
