#pragma once

#include <vector>
#include "Actor.h"
#include <unordered_map>
#include <string>

class World
{
public:
	void TickAllActorSystems(float deltaTime);
	void CleaupAllActors();
	void AddActorSystem(ActorSystem& actorSystem);
	void RemoveActorSystem(EActorSystemID id);
	Actor* FindActorByString(std::wstring name);
	ActorSystem* FindActorSystem(EActorSystemID id);
	ActorSystem* GetActorSystem(unsigned int id);
	Actor* GetActor(unsigned int systemId, unsigned int actorId);

	std::vector<ActorSystem*> actorSystems;
	std::unordered_map<EActorSystemID, ActorSystem*> actorSystemMap; //I thought put a map in for easier finding. Maybe a for loop is just as good

	char name[64];
};

extern World gCurrentWorld;

World* GetWorld();
