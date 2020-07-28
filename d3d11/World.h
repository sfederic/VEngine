#pragma once

#include <vector>
#include "Actor.h"
#include <unordered_map>

class World
{
public:

	void CleaupAllActors();
	void AddActorSystem(ActorSystem& actorSystem);
	void RemoveActorSystem(EActorSystemID id);

	std::vector<ActorSystem> actorSystems;
	std::unordered_map<EActorSystemID, ActorSystem*> actorSystemMap; //I thought put a map in for easier finding. Maybe a for loop is just as good

	char name[64];
};

extern World currentWorld;

World* GetWorld();
