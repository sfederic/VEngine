#pragma once

class ActorSystem;

//This is where every ActorSystem is registered at startup in a hashtable.
//For level loading, TypeFactory is used to call Init()s and whatever else per actor system.
//The map key is the Actor System's name from typeid().
//Could throw this into CoreSystem if CoreSystem is cleanedup a bit. Putting this into a smaller header
//for compile times for now.
class ActorSystemFactory
{
public:

	static void Register(ActorSystem*);

	/*template <class ActorSystemType>
	static void Register(ActorSystem* actorSystem)
	{
		size_t actorSystemID = typeid(ActorSystemType).hash_code();
		std::unordered_map<size_t, ActorSystem*> map;
		map[actorSystemID] = actorSystem;
	}

	static std::unordered_map<size_t, ActorSystem*> actorSystemMap;*/
};
