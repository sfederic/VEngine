export module Components.ComponentSystemCache;

import <typeindex>;
import <string>;
import <vector>;
import <optional>;
import <unordered_map>;

import Core.VAssert;
import Components.IComponentSystem;

export class ComponentSystemCache
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
	std::unordered_map<std::string, IComponentSystem*> nameToSystemMap;
};

void ComponentSystemCache::Add(std::type_index type, IComponentSystem* componentSystem)
{
	VAssert(typeToSystemMap.find(type) == typeToSystemMap.end(), "Type already exists.");
	typeToSystemMap.emplace(type, componentSystem);

	VAssert(nameToSystemMap.find(componentSystem->GetName()) == nameToSystemMap.end(), "Type name already exists.");
	nameToSystemMap.emplace(componentSystem->GetName(), componentSystem);
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
