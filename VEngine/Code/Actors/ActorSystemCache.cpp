#include "ActorSystemCache.h"
#include "IActorSystem.h"

ActorSystemCache actorSystemCache;

void ActorSystemCache::Add(std::type_index type, IActorSystem* actorSystem)
{
	if (actorSystemMap == nullptr)
	{
		actorSystemMap = new std::unordered_map<std::optional<std::type_index>, IActorSystem*>();
	}

	if (nameToSystemMap == nullptr)
	{
		nameToSystemMap = new std::unordered_map<std::string, IActorSystem*>();
	}

	actorSystemMap->insert(std::make_pair(type, actorSystem));
	nameToSystemMap->insert(std::make_pair(actorSystem->name, actorSystem));
}
