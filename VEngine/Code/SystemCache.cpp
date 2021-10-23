#include "SystemCache.h"
#include "System.h"

extern SystemCache systemCache;

void SystemCache::Add(std::type_index type, System* system)
{
	if (typeToSystemMap == nullptr)
	{
		typeToSystemMap = new std::unordered_map<std::optional<std::type_index>, System*>();
	}

	if (nameToSystemMap == nullptr)
	{
		nameToSystemMap = new std::unordered_map<std::string, System*>();
	}

	typeToSystemMap->insert(std::make_pair(type, system));
	nameToSystemMap->insert(std::make_pair(system->name, system));
}

System* SystemCache::Get(std::string systemName)
{
	return nameToSystemMap->find(systemName)->second;
}

System* SystemCache::Get(std::type_index actorType)
{
	return typeToSystemMap->find(actorType)->second;
}
