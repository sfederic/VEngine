#include "ActorSystemFactory.h"
#include "Actor.h"
#include <typeindex>

std::unordered_map<size_t, ActorSystem*> *ActorSystemFactory::IDToSystemMap;
std::unordered_map<ActorSystem*, size_t> *ActorSystemFactory::systemToIDMap;
std::unordered_map<std::string, ActorSystem*> *ActorSystemFactory::nameToSystemMap;
std::unordered_map<std::type_index, ActorSystem*>* ActorSystemFactory::actorTypeToSystemMap;

ActorSystem* activeActorSystemSpawner;

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

ActorSystem* ActorSystemFactory::GetActorSystem(std::string name)
{
	auto actorSystem = nameToSystemMap->find(name);
	return actorSystem->second;
}

ActorSystem* ActorSystemFactory::GetActorSystem(std::type_index actorType)
{
    auto actorSystemIt = actorTypeToSystemMap->find(typeid(actorType));
    if (actorSystemIt != actorTypeToSystemMap->end())
    {
        return actorSystemIt->second;
    }

    return nullptr;
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
	activeActorSystemSpawner = actorSystem;
}

ActorSystem* ActorSystemFactory::GetCurrentActiveActorSystem()
{
	return activeActorSystemSpawner;
}
