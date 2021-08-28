#include "ComponentSystemCache.h"
#include "IComponentSystem.h"

ComponentSystemCache componentSystemCache;

void ComponentSystemCache::Add(std::type_index type, IComponentSystem* componentSystem)
{
	if (componentSystemMap == nullptr)
	{
		componentSystemMap = new std::unordered_map<std::optional<std::type_index>, IComponentSystem*>();
	}

	if (nameToSystemMap == nullptr)
	{
		nameToSystemMap = new std::unordered_map<std::string, IComponentSystem*>();
	}

	componentSystemMap->insert(std::make_pair(type, componentSystem));
	nameToSystemMap->insert(std::make_pair(componentSystem->name, componentSystem));
}
