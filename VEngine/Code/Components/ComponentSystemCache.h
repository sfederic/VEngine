#pragma once

#include <typeindex>
#include <map>
#include <optional>
#include <string>

class IComponentSystem;

class ComponentSystemCache
{
public:
	static ComponentSystemCache& Get()
	{
		static ComponentSystemCache componentSystemCache;
		return componentSystemCache;
	}

	void Add(std::type_index type, IComponentSystem* componentSystem);
	IComponentSystem* GetSystem(std::string systemName);
	IComponentSystem* GetSystem(std::type_index actorType);
	std::vector<IComponentSystem*> GetAllSystems();

private:
	std::unordered_map<std::optional<std::type_index>, IComponentSystem*> typeToSystemMap;
	std::map<std::string, IComponentSystem*> nameToSystemMap;
};
