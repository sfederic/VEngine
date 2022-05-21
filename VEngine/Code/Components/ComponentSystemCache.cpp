#include "vpch.h"
#include "ComponentSystemCache.h"
#include "IComponentSystem.h"

ComponentSystemCache componentSystemCache;

void ComponentSystemCache::Add(std::type_index type, IComponentSystem* componentSystem)
{
	if (typeToSystemMap == nullptr)
	{
		typeToSystemMap = new std::unordered_map<std::optional<std::type_index>, IComponentSystem*>();
	}

	if (nameToSystemMap == nullptr)
	{
		nameToSystemMap = new std::unordered_map<std::string, IComponentSystem*>();
	}

	typeToSystemMap->insert(std::make_pair(type, componentSystem));
	nameToSystemMap->insert(std::make_pair(componentSystem->name, componentSystem));
}

IComponentSystem* ComponentSystemCache::Get(std::string systemName)
{
	return nameToSystemMap->find(systemName)->second;
}

IComponentSystem* ComponentSystemCache::Get(std::type_index actorType)
{
	return typeToSystemMap->find(actorType)->second;
}
