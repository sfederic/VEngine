#pragma once
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <optional>
#include "IActorSystem.h"

namespace ActorSystemCache
{
	std::unordered_map<std::optional<std::type_index>, IActorSystem*>* actorSystemMap = nullptr;

	void Add(std::type_index type, IActorSystem* actorSystem)
	{
		if (actorSystemMap == nullptr)
		{
			actorSystemMap = new std::unordered_map<std::optional<std::type_index>, IActorSystem*>();
		}

		actorSystemMap->insert(std::make_pair(type, actorSystem));
	}
};
