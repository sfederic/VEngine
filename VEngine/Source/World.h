#pragma once

#include <vector>
#include "Actor.h"
#include <unordered_map>
#include <string>

class World
{
public:
	void Load(std::string levelName);
	void TickAllActorSystems(float deltaTime);
	void CleaupAllActors();
	void AddActorSystem(ActorSystem* actorSystem);
	Actor* FindActorByString(std::wstring name);
	ActorSystem* GetActorSystem(unsigned int id);
	Actor* GetActor(unsigned int systemId, unsigned int actorId);

	std::vector<ActorSystem*> actorSystems;
	std::unordered_map<std::wstring, ActorSystem*> actorSystemsMap;

	char name[64];
};

extern World gCurrentWorld;

World* GetWorld();
