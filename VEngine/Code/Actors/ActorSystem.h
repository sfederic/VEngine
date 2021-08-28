#pragma once
#include <vector>
#include "IActorSystem.h"
#include "Actor.h"
#include "ActorSystemCache.h"
#include "Serialiser.h"
#include "Components/Component.h"

template <typename T>
struct ActorSystem : IActorSystem
{
	std::vector<T*> actors;

	ActorSystem()
	{
		std::string typeName = typeid(T).name();
		std::string delimiter = " ";
		std::string token = typeName.substr(typeName.find(delimiter) + 1);
		name = token;

		actorSystemCache.Add(typeid(T), this);
	}

	T* Add(Transform transform = Transform())
	{
		actors.emplace_back(new T());

		T* actor = actors.back();
		actor->actorSystem = this;
		actor->index = actors.size() - 1;
		actor->SetTransform(transform);
		actor->name = this->name + std::to_string(actor->index);

		return actor;
	}

	void Remove(int index)
	{
		for (Component* component : actors[index]->components)
		{
			component->Destroy();
		}

		delete actors[index];
		std::swap(actors[index], actors.back());
		actors[index]->index = index;
		actors.pop_back();
	}

	virtual void Tick(double deltaTime) override
	{
		for (T* actor : actors)
		{
			actor->Tick(deltaTime);
		}
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

	virtual void Deserialise(std::istream& is) override
	{
		for (Actor* actor : actors)
		{
			Serialiser::Deserialise(actor->GetProps(), is);
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

	virtual void Cleanup() override
	{
		for (T* actor : actors)
		{
			delete actor;
		}

		actors.clear();
	}
};

#define ACTOR_SYSTEM(type) inline static ActorSystem<type> system; \
type* Add(Transform transform = Transform()) { return (type*)system.Add(transform); } \
virtual void Destroy() override { system.Remove(index); } \
