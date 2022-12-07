#pragma once

#include <typeindex>
#include <unordered_map>
#include <optional>
#include <string>

class IActorSystem;

class ActorSystemCache
{
public:
	static ActorSystemCache& Get()
	{
		static ActorSystemCache actorSystemCache;
		return actorSystemCache;
	}

	void AddSystem(std::type_index type, IActorSystem* actorSystem);
	IActorSystem* GetSystem(std::string systemName);
	IActorSystem* GetSystem(std::type_index actorType);

	std::vector<IActorSystem*> GetAllSystems();
	std::vector<std::string> GetAllActorSystemNames();

private:
	std::unordered_map<std::optional<std::type_index>, IActorSystem*> typeToSystemMap;
	std::map<std::string, IActorSystem*> nameToSystemMap;
};
