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

		T* actor = actors.back();
		actor->actorSystem = this;

		return actor;
	}

	virtual Actor* SpawnActor(Transform transform) override
	{
		actors.push_back(new T());

		T* actor = actors.back();
		actor->SetTransform(transform);
		actor->actorSystem = this;

		return (Actor*)actor;
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
