#include "ActorSystemCache.h"
#include <unordered_map>
#include <optional>
#include "IActorSystem.h"

std::unordered_map<std::optional<std::type_index>, IActorSystem*>* actorSystemMap = nullptr;

namespace ActorSystemCache
{
	void Add(std::type_index type, IActorSystem* actorSystem)
	{
		if (actorSystemMap == nullptr)
		{
			actorSystemMap = new std::unordered_map<std::optional<std::type_index>, IActorSystem*>();
		}

		actorSystemMap->insert(std::make_pair(type, actorSystem));
	}
};
