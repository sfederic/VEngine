#include "ActorSystemCache.h"
#include "IActorSystem.h"

ActorSystemCache actorSystemCache;

void ActorSystemCache::Add(std::type_index type, IActorSystem* actorSystem)
{
	if (actorSystemMap == nullptr)
	{
		actorSystemMap = new std::unordered_map<std::optional<std::type_index>, IActorSystem*>();
	}

	actorSystemMap->insert(std::make_pair(type, actorSystem));
}
