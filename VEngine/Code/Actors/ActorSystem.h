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
		actorSystemCache.Add(typeid(T), this);
	}

	T* Add()
	{
		actors.push_back(new T());
		actors.back()->actorSystem = this;
		return actors.back();
	}

	virtual Actor* SpawnActor(Transform transform) override
	{
		actors.push_back(new T());
		actors.back()->SetTransform(transform);
		return (Actor*)actors.back();
	}

	virtual void GetActors(std::vector<Actor*>& outActors) override
	{
		for (T* actor : actors)
		{
			outActors.push_back(actor);
		}
	}
};

#define ACTOR_SYSTEM(type) inline static ActorSystem<type> system;
