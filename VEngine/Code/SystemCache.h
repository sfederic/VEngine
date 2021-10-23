#pragma once
#include <unordered_map>
#include <typeindex>
#include <optional>

struct System;

struct SystemCache
{
	std::unordered_map<std::optional<std::type_index>, System*>* typeToSystemMap = nullptr;
	std::unordered_map<std::string, System*>* nameToSystemMap = nullptr;

	void Add(std::type_index type, System* system);
	System* Get(std::string systemName);
	System* Get(std::type_index actorType);
};

extern SystemCache systemCache;
