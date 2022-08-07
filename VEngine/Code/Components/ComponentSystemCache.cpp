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
		nameToSystemMap = new std::map<std::string, IComponentSystem*>();
	}

	typeToSystemMap->insert(std::make_pair(type, componentSystem));
	nameToSystemMap->insert(std::make_pair(componentSystem->name, componentSystem));
}

IComponentSystem* ComponentSystemCache::Get(std::string systemName)
{
	auto csIt = nameToSystemMap->find(systemName);
	if (csIt == nameToSystemMap->end())
	{
		return nullptr;
	}

	return csIt->second;
}

IComponentSystem* ComponentSystemCache::Get(std::type_index actorType)
{
	return typeToSystemMap->find(actorType)->second;
}
