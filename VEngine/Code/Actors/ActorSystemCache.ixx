module Actors.ActorSystemCache;

import <typeindex>;
import <string>;
import <vector>;
import <unordered_map>;
import <optional>;

export class IActorSystem;

export class ActorSystemCache
{
public:
	static ActorSystemCache& Get()
	{
		static ActorSystemCache actorSystemCache;
		return actorSystemCache;
	}

	void AddSystem(std::type_index type, IActorSystem* actorSystem);
	IActorSystem* GetSystem(const std::string& systemName);
	IActorSystem* GetSystem(std::type_index actorType);

	std::vector<IActorSystem*> GetAllSystems() const;
	std::vector<std::string> GetAllActorSystemNames() const;

private:
	std::unordered_map<std::optional<std::type_index>, IActorSystem*> typeToSystemMap;
	std::unordered_map<std::string, IActorSystem*> nameToSystemMap;
};
