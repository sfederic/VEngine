#include "SystemCache.h"
#include "System.h"

SystemCache systemCache;

void SystemCache::Add(System* system)
{
	if (nameToSystemMap == nullptr)
	{
		nameToSystemMap = new std::unordered_map<std::string, System*>();
	}

	nameToSystemMap->insert(std::make_pair(system->name, system));
}

System* SystemCache::Get(std::string systemName)
{
	return nameToSystemMap->find(systemName)->second;
}
