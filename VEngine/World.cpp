#include "World.h"
#include "Actor.h"
#include "Debug.h"
#include "DebugMenu.h"

World gCurrentWorld;

//Returns the current active world
World* GetWorld()
{
	return &gCurrentWorld;
}

void World::TickAllActorSystems()
{
	for (int asIndex = 0; asIndex < actorSystems.size(); asIndex++)
	{
		//TODO: add virtual ticks (actorSystems[i].Tick()
	}
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
		actorSystems.push_back(&actorSystem);
		actorSystemMap[actorSystem.id] = &actorSystem;
	}
	else
	{
		DebugPrint("Actor System already exists in world. Cannot create.\n");
		debugMenu.AddNotification(L"Cannot create actor system.");
	}
}

void World::RemoveActorSystem(EActorSystemID id)
{
	auto actorSystemIt = actorSystemMap.find(id);
	if (actorSystemIt == actorSystemMap.end())
	{
		DebugPrint("Actor system ID: %d not found to remove.\n", id);
		debugMenu.AddNotification(L"Actor system not found to remove.");
		return;
	}

	for (int i = 0; i < actorSystems.size(); i++)
	{
		if (i == (int)actorSystems[i]->id)
		{
			actorSystems.erase(actorSystems.begin() + i);
			actorSystemMap.erase(id);
			break;
		}
	}
}

ActorSystem* World::FindActorSystem(EActorSystemID id)
{
	auto actorSystemIt = actorSystemMap.find(id);
	if (actorSystemIt == actorSystemMap.end())
	{
		DebugPrint("Actor system not found in GetActorSystem/n");
	}
	else
	{
		return actorSystemIt->second;
	}
}

ActorSystem* World::GetActorSystem(unsigned int id)
{
	if (id < actorSystems.size())
	{
		return actorSystems[id];
	}

	return nullptr;
}

Actor* World::GetActor(unsigned int systemId, unsigned int actorId)
{
	if (systemId < actorSystems.size())
	{
		if (actorId < actorSystems[systemId]->actors.size())
		{
			return &actorSystems[systemId]->actors[actorId];
		}
	}

	return nullptr;
}
