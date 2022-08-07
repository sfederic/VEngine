#include "vpch.h"
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
		nameToSystemMap = new std::map<std::string, IActorSystem*>();
	}

	typeToSystemMap->insert(std::make_pair(type, actorSystem));
	nameToSystemMap->insert(std::make_pair(actorSystem->GetName(), actorSystem));
}

IActorSystem* ActorSystemCache::Get(std::string systemName)
{
	auto asIt = nameToSystemMap->find(systemName);
	if (asIt == nameToSystemMap->end())
	{
		return nullptr;
	}

	return asIt->second;
}

IActorSystem* ActorSystemCache::Get(std::type_index actorType)
{
	return typeToSystemMap->find(actorType)->second;
}
