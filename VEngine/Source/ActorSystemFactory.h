#pragma once

#include <unordered_map>

class ActorSystem;

//TODO: look into precompiled headers for files like this thrown onto actors.
//Gonna need a 'CoreMinimal.h'-tier solution here.

//This is where every ActorSystem is registered at startup in a hashtable.
//For level loading, ActorSystemFactory is used to call Init()s and whatever else per actor system.
//The map key is the Actor System's name from typeid().
//Could throw this into CoreSystem if CoreSystem is cleanedup a bit. Putting this into a smaller header
//for compile times for now.
class ActorSystemFactory
{
public:
	ActorSystemFactory();

	template <class ActorSystemType>
	static void Register(ActorSystem* actorSystem)
	{
		//There's a lot of weird stuff with static initialisation order and C++
		//and how std::maps involve in that. Have to make them pointers.
		if (IDToSystemMap == nullptr)
		{
			IDToSystemMap = new std::unordered_map<size_t, ActorSystem*>;
		}
		if (systemToIDMap == nullptr)
		{
			systemToIDMap = new std::unordered_map<ActorSystem*, size_t>;
		}
		if (nameToSystemMap == nullptr)
		{
			nameToSystemMap = new std::unordered_map<const wchar_t*, ActorSystem*>;
		}

		size_t id = typeid(ActorSystemType).hash_code();
		IDToSystemMap->insert(std::pair(id, actorSystem));
		systemToIDMap->insert(std::pair(actorSystem, id));
		nameToSystemMap->insert(std::pair(actorSystem->name.c_str(), actorSystem));
	}

	static size_t GetActorSystemID(ActorSystem* actorSystem);
	static ActorSystem* GetActorSystem(size_t id);
	static ActorSystem* GetActorSystem(const wchar_t* name);
	static void GetAllActorSystems(std::vector<ActorSystem*>& actorSystems);
	static void SetCurrentActiveActorSystem(ActorSystem* actorSystem);

	static std::unordered_map<size_t, ActorSystem*> *IDToSystemMap;
	static std::unordered_map<ActorSystem*, size_t> *systemToIDMap;
	static std::unordered_map<const wchar_t*, ActorSystem*>* nameToSystemMap;
};
