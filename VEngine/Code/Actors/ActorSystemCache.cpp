#include "vpch.h"
#include "ActorSystemCache.h"
#include "IActorSystem.h"

void ActorSystemCache::AddSystem(std::type_index type, IActorSystem* actorSystem)
{
	typeToSystemMap.emplace(type, actorSystem);
	nameToSystemMap.emplace(actorSystem->GetName(), actorSystem);
}

IActorSystem* ActorSystemCache::GetSystem(std::string systemName)
{
	auto asIt = nameToSystemMap.find(systemName);
	if (asIt == nameToSystemMap.end())
	{
		return nullptr;
	}

	return asIt->second;
}

IActorSystem* ActorSystemCache::GetSystem(std::type_index actorType)
{
	return typeToSystemMap.find(actorType)->second;
}

std::vector<IActorSystem*> ActorSystemCache::GetAllSystems()
{
	std::vector<IActorSystem*> systems;
	for (const auto& [key, value] : nameToSystemMap)
	{
		systems.emplace_back(value);
	}
	return systems;
}

std::vector<std::string> ActorSystemCache::GetAllActorSystemNames()
{
	std::vector<std::string> names;
	for (auto& systemPair : nameToSystemMap)
	{
		names.emplace_back(systemPair.first);
	}
	return names;
}
