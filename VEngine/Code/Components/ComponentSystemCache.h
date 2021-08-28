#pragma once
#include <typeindex>
#include <unordered_map>
#include <optional>
#include <string>

struct IComponentSystem;

struct ComponentSystemCache
{
	std::unordered_map<std::optional<std::type_index>, IComponentSystem*>* componentSystemMap = nullptr;
	std::unordered_map<std::string, IComponentSystem*>* nameToSystemMap = nullptr;

	void Add(std::type_index type, IComponentSystem* componentSystem);
};

extern ComponentSystemCache componentSystemCache;
