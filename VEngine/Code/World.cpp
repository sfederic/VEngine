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
		std::vector<Actor*> actors;
		actorSystem->GetActors(actors);

		for (Actor* actor : actors)
		{
			if (actor->name == actorName)
			{
				return actor;
			}
		}
	}

	return nullptr;
}
