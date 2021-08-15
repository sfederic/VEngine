#pragma once
#include <vector>
#include "IActorSystem.h"
#include "Actor.h"
#include "ActorSystemCache.h"

template <typename T>
struct ActorSystem : IActorSystem
{
	std::vector<T*> actors;

	ActorSystem()
	{
		ActorSystemCache::Add(typeid(T), this);
	}

	T* Add()
	{
		actors.push_back(new T());
		return actors.back();
	}

	virtual void GetActors(std::vector<Actor*>& outActors) override
	{
		for (T* actor : actors)
		{
			outActors.push_back(actor);
		}
	}
};

#define ACTOR_SYSTEM(type) \
inline static ActorSystem<type> system; \
