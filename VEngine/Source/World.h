#pragma once

#include <vector>
#include "Actor.h"
#include <unordered_map>
#include <string>
#include "ActorSystemFactory.h"

class World
{
public:
	void Load(std::string levelName);
	void TickAllActorSystems(float deltaTime);
	void CleaupAllActors();
	void AddActorSystem(ActorSystem* actorSystem);

	template <class ActorType>
	Actor* SpawnActor(Transform transform)
	{
		ActorSystem* actorSystem = ActorSystemFactory::GetActorSystem(typeid(ActorType));
		if (actorSystem)
		{
			actorSystem->SpawnActor(transform);
		}
	}

	template <class ActorType>
	void SpawnActors(std::vector<Transform>& transforms, uint64_t numActorsToSpawn)
	{
		ActorSystem* actorSystem = ActorSystemFactory::GetActorSystem(typeid(ActorType));
		if (actorSystem)
		{
			actorSystem->SpawnActors(transforms, numActorsToSpawn);
		}
	}

	Actor* FindActorByString(const std::string& name);
	ActorSystem* GetActorSystem(unsigned int id);
	Actor* GetActor(unsigned int systemId, unsigned int actorId);

	std::vector<ActorSystem*> actorSystems;
	std::unordered_map<std::string, ActorSystem*> actorSystemsMap;

	char name[64];
};

extern World gCurrentWorld;

World* GetWorld();
