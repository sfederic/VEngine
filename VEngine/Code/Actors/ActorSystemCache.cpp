import Actors.ActorSystemCache;
import Actors.IActorSystem;
import Core.VAssert;

import <typeindex>;
import <string>;
import <vector>;
import <unordered_map>;

void ActorSystemCache::AddSystem(std::type_index type, IActorSystem* actorSystem)
{
	VAssert(typeToSystemMap.find(type) == typeToSystemMap.end(), "");
	typeToSystemMap.emplace(type, actorSystem);

	VAssert(nameToSystemMap.find(actorSystem->GetName()) == nameToSystemMap.end(), "");
	nameToSystemMap.emplace(actorSystem->GetName(), actorSystem);
}

IActorSystem* ActorSystemCache::GetSystem(const std::string& systemName)
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

std::vector<IActorSystem*> ActorSystemCache::GetAllSystems() const
{
	std::vector<IActorSystem*> systems;
	systems.reserve(nameToSystemMap.size());
	for (const auto& [_, value] : nameToSystemMap)
	{
		systems.emplace_back(value);
	}
	return systems;
}

std::vector<std::string> ActorSystemCache::GetAllActorSystemNames() const
{
	std::vector<std::string> names;
	names.reserve(nameToSystemMap.size());
	for (const auto& systemPair : nameToSystemMap)
	{
		names.emplace_back(systemPair.first);
	}
	return names;
}
