#pragma once

#include <vector>
#include <memory>
#include <unordered_set>
#include "IActorSystem.h"
#include "Actor.h"
#include "ActorSystemCache.h"
#include "Core/Serialiser.h"
#include "Core/Deserialiser.h"
#include "Core/BinarySerialiser.h"
#include "Core/BinaryDeserialiser.h"
#include "Components/Component.h"
#include "Editor/Editor.h"
#include "Core/VString.h"
#include "Core/World.h"

//Actor systems were based on UE4 talk from Rare
//Ref: https://www.unrealengine.com/en-US/events/unreal-fest-europe-2019/aggregating-ticks-to-manage-scale-in-sea-of-thieves

template <typename T>
class ActorSystem : public IActorSystem
{
public:
	ActorSystem()
	{
		std::string typeName = typeid(T).name();
		std::string token = typeName.substr(typeName.find(" ") + 1);
		_name = token;

		ActorSystemCache::Get().AddSystem(typeid(T), this);
	}

	T* Add(Transform transform = Transform())
	{
		actors.emplace_back(std::make_unique<T>());
		auto& actor = actors.back();

		actor->SetActorSystem(this);
		actor->SetSystemIndex(actors.size() - 1);
		actor->SetTransform(transform);

		//AddActorToWorld() call is in SetName()
		if (!actor->SetName(this->GetName() + std::to_string(actor->GetSystemIndex())))
		{
			auto uid = (unsigned int)GenerateUID(); //std::to_string goes bananas with UID
			actor->SimpleSetName(this->GetName() + std::to_string(uid));
		}

		return actor.get();
	}

	void Remove(size_t index)
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

		World::Get().RemoveActorFromWorld(actors.back().get());
		actors.pop_back();

		Editor::Get().UpdateWorldList();
		Editor::Get().ClearProperties();
	}

	virtual void RemoveInterfaceActor(Actor* actor) override
	{
		Remove(actor->GetSystemIndex());
	}

	void RemoveAllActors()
	{
		for (int i = actors.size() - 1; i > -1; i--)
		{
			Remove(actors[i]->GetSystemIndex());
		}
	}

	virtual void Tick(float deltaTime) override
	{
		//Has to be an index based forloop here instead of a range forloop because 
		//actors can be destroyed in-game and its pointer popped off its actorsystem.
		for (int i = 0; i < actors.size(); i++)
		{
			if (actors[i]->IsTickEnabled() && actors[i]->IsActive())
			{
				actors[i]->Tick(deltaTime);
			}
		}
	}

	void Init() override
	{
		for (auto& actor : actors)
		{
			actor->Create();
		}
	}

	void PostInit() override
	{
		for (auto& actor : actors)
		{
			actor->PostCreate();
		}
	}

	virtual void CreateAllActorComponents() override
	{
		for (auto& actor : actors)
		{
			actor->CreateAllComponents();
		}
	}

	std::vector<std::unique_ptr<T>>& GetActors()
	{
		return actors;
	}

	T* GetFirstActor()
	{
		if (actors.empty())
		{
			return nullptr;
		}

		return actors.front().get();
	}

	T* GetOnlyActor()
	{
		assert(actors.size() == 1);
		return actors.front().get();
	}

	virtual size_t GetNumActors() override
	{
		return actors.size();
	}

	virtual void Serialise(Serialiser& s) override
	{
		s.WriteLine(VString::stows(GetName())); //Use actorsystem name to create again from ActorSystemCache on Deserialise
		s.WriteLine(actors.size()); //Write out num of actors to load the same amount on Deserialise

		for (auto& actor : actors)
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

		for (auto& actor : actors)
		{
			auto props = actor->GetProps();
			s.Serialise(props);
		}
	}

	virtual void Deserialise(Deserialiser& d) override
	{
		for (auto& actor : actors)
		{
			auto props = actor->GetProps();
			d.Deserialise(props);
		}
	}

	virtual void DeserialiseBinary(BinaryDeserialiser& d) override
	{
		for (auto& actor : actors)
		{
			auto props = actor->GetProps();
			d.Deserialise(props);
		}
	}

	virtual Actor* FindActorByName(std::string actorName) override
	{
		for (auto& actor : actors)
		{
			if (actor->GetName() == actorName)
			{
				return (Actor*)actor.get();
			}
		}

		return nullptr;
	}

	virtual Actor* SpawnActor(const Transform& transform) override
	{
		auto actor = static_cast<Actor*>(Add(transform));
		return actor;
	}

	virtual std::vector<Actor*> GetActorsAsBaseClass() override
	{
		std::vector<Actor*> outActors;
		outActors.reserve(actors.size());
		for (auto& actor : actors)
		{
			outActors.emplace_back(actor.get());
		}
		return outActors;
	}

	virtual void Cleanup() override
	{
		actors.clear();
	}

	virtual void DeferActorForDestroy(size_t index) override
	{
		actorIndexToDeferDestroy.insert(index);
	}

	virtual void DestroyDeferredActors() override
	{
		for (auto index : actorIndexToDeferDestroy)
		{
			Remove(index);
		}

		actorIndexToDeferDestroy.clear();
	}

	virtual void DestroyAll() override
	{
		for (auto& actor : actors)
		{
			actor->Destroy();
		}
	}

	void AddDeletedActor(Actor* actorToDelete) override
	{
		auto duplicateActor = new T();
		duplicateActor->SetActorSystem(this);

		auto newActorProps = duplicateActor->GetAllProps();
		auto deletedActorProps = actorToDelete->GetAllProps();
		Properties::CopyProperties(deletedActorProps, newActorProps);

		duplicateActor->Create();
		duplicateActor->CreateAllComponents();
		duplicateActor->PostCreate();

		deletedActors.push_back(duplicateActor);
	}

	Actor* GetLastDeletedActor() override
	{
		if (deletedActors.empty())
		{
			return nullptr;
		}
		return (Actor*)deletedActors.back();
	}

	void PopBackLastDeletedActor() override
	{
		auto actor = deletedActors.back();

		//Temporarily add actor to world so that the component deletes can find the actor by UID in world.
		World::Get().AddActorToWorld(actor);

		auto components = actor->GetAllComponents();
		for (int i = 0; i < components.size(); i++)
		{
			Component* component = components[i];
			actor->RemoveComponent(component);
			component->Remove();
		}

		World::Get().RemoveActorFromWorld(actor);

		//This will cause leaks calling this and not cleaning the actor up properly,
		//it shouldn't matter too much as it's just in the editor but keep an eye on it.
		deletedActors.pop_back();
	}

private:
	std::vector<std::unique_ptr<T>> actors;
	std::vector<Actor*> deletedActors;
	std::unordered_set<size_t> actorIndexToDeferDestroy;
};

#define ACTOR_SYSTEM(type) inline static ActorSystem<type> system; \
virtual void Remove() override { __super::Remove(); Destroy(); system.Remove(GetSystemIndex()); } \
