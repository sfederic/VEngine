#pragma once
#include <vector>
#include "IActorSystem.h"
#include "Actor.h"
#include "ActorSystemCache.h"
#include "Serialiser.h"

template <typename T>
struct ActorSystem : IActorSystem
{
	std::vector<T*> actors;

	ActorSystem()
	{
		actorSystemCache.Add(typeid(T), this);

		std::string typeName = typeid(T).name();
		std::string delimiter = " ";
		std::string token = typeName.substr(typeName.find(delimiter) + 1);
		name = token;
	}

	T* Add(Transform transform = Transform())
	{
		actors.push_back(new T());

		T* actor = actors.back();
		actor->actorSystem = this;
		actor->SetTransform(transform);
		actor->name = this->name + std::to_string(actors.size() - 1);

		return actor;
	}

	virtual void Serialise(std::ostream& os) override
	{
		os << name << "\n"; //Use actorsystem name to create again from ActorSystemCache on Deserialise
		os << actors.size() << "\n"; //Write out num of actors to load the same amount on Deserialise

		for (T* actor : actors)
		{
			Serialiser::Serialise(actor->GetProps(), os);
		}
	}

	virtual Actor* FindActorByName(std::string actorName) override
	{
		for (T* actor : actors)
		{
			if (actor->name == actorName)
			{
				return (Actor*)actor;
			}
		}

		return nullptr;
	}

	virtual Actor* SpawnActor(Transform transform) override
	{
		Actor* actor = (Actor*)Add(transform);
		return actor;
	}

	virtual void GetActors(std::vector<Actor*>& outActors) override
	{
		for (T* actor : actors)
		{
			outActors.push_back(actor);
		}
	}
};

#define ACTOR_SYSTEM(type) inline static ActorSystem<type> system; \
type* Add(Transform transform = Transform()) { return (type*)system.Add(transform); } \
