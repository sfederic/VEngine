#include "ActorSystemCache.h"
#include "IActorSystem.h"

ActorSystemCache actorSystemCache;

void ActorSystemCache::Add(std::type_index type, IActorSystem* actorSystem)
{
	if (typeToSystemMap == nullptr)
	{
		typeToSystemMap = new std::unordered_map<std::optional<std::type_index>, IActorSystem*>();
	}

	if (nameToSystemMap == nullptr)
	{
		nameToSystemMap = new std::unordered_map<std::string, IActorSystem*>();
	}

	typeToSystemMap->insert(std::make_pair(type, actorSystem));
	nameToSystemMap->insert(std::make_pair(actorSystem->name, actorSystem));
}

IActorSystem* ActorSystemCache::Get(std::string systemName)
{
	return nameToSystemMap->find(systemName)->second;
}

IActorSystem* ActorSystemCache::Get(std::type_index actorType)
{
	return typeToSystemMap->find(actorType)->second;
}
