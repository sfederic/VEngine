#pragma once
#include <typeindex>
#include <map>
#include <optional>
#include <string>

struct IComponentSystem;

struct ComponentSystemCache
{
	std::unordered_map<std::optional<std::type_index>, IComponentSystem*>* typeToSystemMap = nullptr;
	std::map<std::string, IComponentSystem*>* nameToSystemMap = nullptr;

	void Add(std::type_index type, IComponentSystem* componentSystem);
	IComponentSystem* Get(std::string systemName);
	IComponentSystem* Get(std::type_index actorType);
};

extern ComponentSystemCache componentSystemCache;
