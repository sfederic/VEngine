#include "vpch.h"
#include "ComponentSystemCache.h"
#include "IComponentSystem.h"

void ComponentSystemCache::Add(std::type_index type, IComponentSystem* componentSystem)
{
	typeToSystemMap.insert(std::make_pair(type, componentSystem));
	nameToSystemMap.insert(std::make_pair(componentSystem->name, componentSystem));
}

IComponentSystem* ComponentSystemCache::GetSystem(std::string systemName)
{
	auto csIt = nameToSystemMap.find(systemName);
	if (csIt == nameToSystemMap.end())
	{
		return nullptr;
	}

	return csIt->second;
}

IComponentSystem* ComponentSystemCache::GetSystem(std::type_index actorType)
{
	return typeToSystemMap.find(actorType)->second;
}

std::vector<IComponentSystem*> ComponentSystemCache::GetAllSystems()
{
	std::vector<IComponentSystem*> systems;
	for (const auto& [key, value] : nameToSystemMap)
	{
		systems.emplace_back(value);
	}
	return systems;
}
