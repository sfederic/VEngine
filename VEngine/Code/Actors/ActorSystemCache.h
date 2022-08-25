#pragma once

#include <typeindex>
#include <unordered_map>
#include <optional>
#include <string>

class IActorSystem;

struct ActorSystemCache
{
	//Maps are pointers because of static init order.
	std::unordered_map<std::optional<std::type_index>, IActorSystem*>* typeToSystemMap = nullptr;
	std::map<std::string, IActorSystem*>* nameToSystemMap = nullptr;

	void Add(std::type_index type, IActorSystem* actorSystem);
	IActorSystem* Get(std::string systemName);
	IActorSystem* Get(std::type_index actorType);

	std::vector<std::string> GetAllActorSystemNames();
};

extern ActorSystemCache actorSystemCache;
