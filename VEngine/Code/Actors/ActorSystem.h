#pragma once
#include <vector>
#include "IActorSystem.h"
#include "Actor.h"
#include "ActorSystemCache.h"
#include "Serialiser.h"
#include "Components/Component.h"
#include "Editor/Editor.h"
#include "World.h"
#include "VString.h"

//Actor systems were based on UE4 talk from Rare
//Ref: https://www.unrealengine.com/en-US/events/unreal-fest-europe-2019/aggregating-ticks-to-manage-scale-in-sea-of-thieves

template <typename T>
class ActorSystem : IActorSystem
{
	std::vector<T*> actors;

public:
	ActorSystem()
	{
		std::string typeName = typeid(T).name();
		std::string token = typeName.substr(typeName.find(" ") + 1);
		name = token;

		actorSystemCache.Add(typeid(T), this);
	}

	T* Add(T newActor = T(), Transform transform = Transform())
	{
		T* actor = new T(std::move(newActor));
		actors.emplace_back(actor);

		actor->SetActorSystem(this);
		actor->SetSystemIndex(actors.size() - 1);
		actor->SetTransform(transform);
		actor->SimpleSetName(this->name + std::to_string(actor->GetSystemIndex()));

		World::AddActorToWorld(actor);

		return actor;
	}

	void Remove(int index)
	{
		auto components = actors[index]->GetAllComponents();
		for (int i = 0; i < components.size(); i++)
		{
			Component* component = components[i];
			actors[index]->RemoveComponent(component);
			component->Remove();
		}

		std::swap(actors[index], actors.back());
		actors[index]->SetSystemIndex(index);
		actors[index]->SetName(GetName() + std::to_string(index));

		World::RemoveActorFromWorld(actors.back());

		delete actors.back();
		actors.pop_back();

		//@Todo: just keep an eye on this. Code like this makes me want to seperate editor from game.
		//It shouldn't be too bad with Release/NoEditor config though, maybe through an #ifdef here later.
		editor->UpdateWorldList();
		editor->ClearProperties();
	}

	virtual void Tick(float deltaTime) override
	{
		//Has to be an index based forloop here instead of a range forloop because 
		//actors can be destroyed in-game and its pointer popped off its actorsystem.
		for (int i = 0; i < actors.size(); i++)
		{
			if (actors[i]->IsActive())
			{
				actors[i]->Tick(deltaTime);
			}
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

	std::vector<T*>& GetActors() 
	{
		return actors;
	}

	T* GetFirstActor()
	{
		if (actors.empty())
		{
			return nullptr;
		}

		return actors.front();
	}

	virtual uint32_t GetNumActors() override
	{
		return actors.size();
	}

	virtual void Serialise(Serialiser& s) override
	{
		s.WriteLine(VString::stows(GetName())); //Use actorsystem name to create again from ActorSystemCache on Deserialise
		s.WriteLine(actors.size()); //Write out num of actors to load the same amount on Deserialise

		for (T* actor : actors)
		{
			auto props = actor->GetProps();
			//actor->ResetOwnerUIDToComponents();
			s.Serialise(props);
			s.WriteLine(L"next");
		}
	}

	virtual void SerialiseBinary(BinarySerialiser& s) override
	{
		s.WriteString(GetName());
		size_t numActors = actors.size();
		s.Write(&numActors);

		for (T* actor : actors)
		{
			auto props = actor->GetProps();
			s.Serialise(props);
		}
	}

	virtual void Deserialise(Deserialiser& d) override
	{
		for (T* actor : actors)
		{
			auto props = actor->GetProps();
			d.Deserialise(props);
		}
	}

	virtual void DeserialiseBinary(BinaryDeserialiser& d) override
	{
		for (T* actor : actors)
		{
			auto props = actor->GetProps();
			d.Deserialise(props);
		}
	}

	virtual Actor* FindActorByName(std::string actorName) override
	{
		for (T* actor : actors)
		{
			if (actor->GetName() == actorName)
			{
				return (Actor*)actor;
			}
		}

		return nullptr;
	}

	virtual Actor* SpawnActor(Transform transform) override
	{
		Actor* actor = (Actor*)Add(T(), transform);
		editor->AddActorToWorldList(actor);
		return actor;
	}

	virtual std::vector<Actor*> GetActorsAsBaseClass() override
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
virtual void Destroy() override { system.Remove(GetSystemIndex()); } \
