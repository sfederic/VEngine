#include "ActorSystemFactory.h"
#include "Debug.h"

std::unordered_map<size_t, ActorSystem*> *ActorSystemFactory::IDToSystemMap;
std::unordered_map<ActorSystem*, size_t> *ActorSystemFactory::systemToIDMap;

size_t ActorSystemFactory::GetActorSystemID(ActorSystem* actorSystem)
{
	auto ID = systemToIDMap->find(actorSystem);
	return ID->second;
}

ActorSystem* ActorSystemFactory::GetActorSystem(size_t id)
{
	auto actorSystem = IDToSystemMap->find(id);
	return actorSystem->second;
}

void ActorSystemFactory::GetAllActorSystems(std::vector<ActorSystem*>& actorSystems)
{
	for (auto& as : *IDToSystemMap)
	{
		actorSystems.push_back(as.second);
	}
}
