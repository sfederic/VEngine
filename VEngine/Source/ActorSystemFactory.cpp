#include "ActorSystemFactory.h"
#include "Debug.h"
#include "Actor.h"

std::unordered_map<size_t, ActorSystem*> *ActorSystemFactory::IDToSystemMap;
std::unordered_map<ActorSystem*, size_t> *ActorSystemFactory::systemToIDMap;
std::unordered_map<std::wstring, ActorSystem*> *ActorSystemFactory::nameToSystemMap;

//The currently selected actor system to spawn actors from.
ActorSystem* currentActiveActorSystem;

ActorSystemFactory::ActorSystemFactory()
{

}

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

ActorSystem* ActorSystemFactory::GetActorSystem(std::wstring name)
{
	auto actorSystem = nameToSystemMap->find(name);
	return actorSystem->second;
}

void ActorSystemFactory::GetAllActorSystems(std::vector<ActorSystem*>& actorSystems)
{
	for (auto& as : *IDToSystemMap)
	{
		actorSystems.push_back(as.second);
	}
}

void ActorSystemFactory::SetCurrentActiveActorSystem(ActorSystem* actorSystem)
{
	currentActiveActorSystem = actorSystem;
}
