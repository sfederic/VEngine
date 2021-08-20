#include "World.h"
#include "Actors/NormalActor.h"

World world;

void World::Start()
{
	activeActorSystems.push_back(&NormalActor::system);
}

Actor* World::FindActorByName(std::string actorName)
{
	for (IActorSystem* actorSystem : activeActorSystems)
	{
		Actor* foundActor = actorSystem->FindActorByName(actorName);

		if (foundActor)
		{
			return foundActor;
		}
	}

	return nullptr;
}

void World::CleanupActorSystems()
{
	for (IActorSystem* actorSystem : activeActorSystems)
	{
		actorSystem->Clean();
	}

	activeActorSystems.clear();
}
