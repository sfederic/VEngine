#include "World.h"
#include "Actor.h"
#include "Debug.h"

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

void World::AddActorSystem(ActorSystem& actorSystem)
{
	auto actorSystemIt = actorSystemMap.find(actorSystem.id);
	if (actorSystemIt == actorSystemMap.end())
	{
		actorSystems.push_back(actorSystem);
		actorSystemMap[actorSystem.id] = &actorSystem;
	}
	else
	{
		DebugPrint("Actor System already exists in world. Cannot create.\n");
	}
}

void World::RemoveActorSystem(EActorSystemID id)
{
	auto actorSystemIt = actorSystemMap.find(id);
	if (actorSystemIt == actorSystemMap.end())
	{
		DebugPrint("Actor system ID: %d not found to remove.\n", id);
		return;
	}

	for (int i = 0; i < actorSystems.size(); i++)
	{
		if (i == (int)actorSystems[i].id)
		{
			actorSystems.erase(actorSystems.begin() + i);
			actorSystemMap.erase(id);
			break;
		}
	}
}
