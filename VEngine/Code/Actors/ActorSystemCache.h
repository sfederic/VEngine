#pragma once
#include <typeindex>
#include <unordered_map>
#include <optional>

struct IActorSystem;

struct ActorSystemCache
{
	std::unordered_map<std::optional<std::type_index>, IActorSystem*>* actorSystemMap;

	void Add(std::type_index type, IActorSystem* actorSystem);
};

extern ActorSystemCache actorSystemCache;
