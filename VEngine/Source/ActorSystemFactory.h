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
	template <class ActorSystemType>
	static void Register(ActorSystem* actorSystem)
	{
		size_t id = typeid(ActorSystemType).hash_code();
		actorSystemMap.insert(std::pair(id, actorSystem));
	}

	static std::unordered_map<size_t, ActorSystem*> actorSystemMap;
};
