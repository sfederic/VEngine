#pragma once
#include <typeindex>
#include <unordered_map>
#include <optional>
#include <string>

struct IActorSystem;

struct ActorSystemCache
{
	std::unordered_map<std::optional<std::type_index>, IActorSystem*>* actorSystemMap = nullptr;
	std::unordered_map<std::string, IActorSystem*>* nameToSystemMap = nullptr;

	void Add(std::type_index type, IActorSystem* actorSystem);
};

extern ActorSystemCache actorSystemCache;
