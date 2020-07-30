#pragma once

#include <vector>
#include "Actor.h"
#include <unordered_map>

//Mario Bros. used the term World. One world per time for this engine. No streaming. No cross world functionality.
class World
{
public:

	void TickAllActorSystems();
	void CleaupAllActors();
	void AddActorSystem(ActorSystem& actorSystem);
	void RemoveActorSystem(EActorSystemID id);
	ActorSystem* FindActorSystem(EActorSystemID id);
	ActorSystem* GetActorSystem(unsigned int id);
	Actor* GetActor(unsigned int systemId, unsigned int actorId);

	std::vector<ActorSystem> actorSystems;
	std::unordered_map<EActorSystemID, ActorSystem*> actorSystemMap; //I thought put a map in for easier finding. Maybe a for loop is just as good

	char name[64];
};

extern World gCurrentWorld;

World* GetWorld();
