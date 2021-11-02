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
		std::string token = typeName.substr(typeName.find(" ") + 1);
		name = token;

		actorSystemCache.Add(typeid(T), this);
	}

	T* Add(T newActor = T(), Transform transform = Transform())
	{
		T* actor = new T(newActor);
		actors.push_back(actor);

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

		std::swap(actors[index], actors.back());
		actors[index]->index = index;
		actors[index]->name = this->name + std::to_string(index);
		delete actors.back();
		actors.pop_back();
	}

	virtual void Tick(float deltaTime) override
	{
		//Has to be an index based forloop here because actors can be destroyed in-game
		//and its pointer popped off its actorsystem.
		for (int i = 0; i < actors.size(); i++)
		{
			actors[i]->Tick(deltaTime);
		}
	}

	virtual void Init() override
	{
		for (T* actor : actors)
		{
			actor->Create();
		}
	}

	virtual void CreateAllActorComponents() override
	{
		for (T* actor : actors)
		{
			actor->CreateAllComponents();
		}
	}

	virtual void Serialise(Serialiser& s) override
	{
		s.WriteLine(name); //Use actorsystem name to create again from ActorSystemCache on Deserialise
		s.WriteLine(actors.size()); //Write out num of actors to load the same amount on Deserialise

		for (T* actor : actors)
		{
			Properties mergedProps;

			auto propsVector = actor->GetAllProps();
			for (auto props : propsVector)
			{
				mergedProps.Merge(props);
			}

			s.Serialise(mergedProps);
		}
	}

	virtual void Deserialise(Deserialiser& d) override
	{
		for (T* actor : actors)
		{
			Properties mergedProps;

			auto propsVector = actor->GetAllProps();
			for (auto props : propsVector)
			{
				mergedProps.Merge(props);
			}

			d.Deserialise(mergedProps);
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
		Actor* actor = (Actor*)Add(T(), transform);
		//actor->CreateAllComponents();
		return actor;
	}

	virtual std::vector<Actor*> GetActors() override
	{
		std::vector<Actor*> outActors;

		for (T* actor : actors)
		{
			outActors.push_back(actor);
		}

		return outActors;
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
virtual void Destroy() override { system.Remove(index); } \
