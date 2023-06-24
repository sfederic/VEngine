#pragma once

#include <vector>
#include <string>
#include "UID.h"
#include "Actors/ActorSystemCache.h"

class IActorSystem;
class IComponentSystem;
class Component;
class Actor;

namespace World
{
	extern std::string worldFilename;

	extern std::map<UID, Actor*> actorUIDMap;
	extern std::map<std::string, Actor*> actorNameMap;

	extern std::vector<IActorSystem*> activeActorSystems;
	extern std::vector<IComponentSystem*> activeComponentSystems;

	void Init();

	//Called on level load
	void Start();

	//Called when gameplay begins
	void WakeAndStartAllActors();
	void StartAllComponents();

	//Call End() on all actors on gameplay end
	void EndAllActors();

	void DestroyAllDeferredActors();

	//Create default starting actors for a map.
	void CreateDefaultMapActors();

	std::vector<IActorSystem*> GetLayerActorSystems();

	void TickAllActorSystems(float deltaTime);
	void TickAllComponentSystems(float deltaTime);
	std::vector<Actor*> GetAllActorsInWorld();

	Actor* GetActorByUID(UID uid);
	Actor* GetActorByUIDAllowNull(UID uid);
	Actor* GetActorByName(std::string actorName);
	Actor* GetActorByNameAllowNull(std::string actorName);

	std::vector<Component*> GetAllComponentsInWorld();

	void AddActorToWorld(Actor* actor);

	void RemoveActorFromWorld(Actor* actor);
	void RemoveActorFromWorld(UID actorUID);
	void RemoveActorFromWorld(std::string actorName);

	void ClearAllActorsFromWorld();

	bool CheckIfActorExistsInWorld(std::string actorName);
	bool CheckIfActorExistsInWorld(UID actorUID);

	void Cleanup();

	template <typename T>
	std::vector<T*> GetAllActorsOfTypeInWorld()
	{
		auto actorSystem = ActorSystemCache::Get().GetSystem(typeid(T));
		auto actors = actorSystem->GetActorsAsBaseClass();
		std::vector<T*> outActors;
		for (auto actor : actors)
		{
			outActors.push_back(dynamic_cast<T*>(actor));
		}
		return outActors;
	}

	template <typename T>
	std::vector<T*> GetAllComponentsOfType()
	{
		std::vector<T*> outComponents;
		auto components = GetAllComponentsInWorld();
		for (auto component : components)
		{
			auto componentType = dynamic_cast<T*>(component);
			if (componentType)
			{
				outComponents.push_back(componentType);
			}
		}
		return outComponents;
	}

	template <typename T>
	std::vector<T*> GetAllComponentSystemsOfType()
	{
		std::vector<T*> outComponentSystems;

		for (auto componentSystem : activeComponentSystems)
		{
			auto systemType = dynamic_cast<T*>(componentSystem);
			if (systemType)
			{
				outComponentSystems.emplace_back(systemType);
			}
		}

		return outComponentSystems;
	}

	template <typename T>
	std::vector<Actor*> GetAllActorsOfTypeAsActor()
	{
		std::vector<Actor*> outActors;

		auto actors = GetAllActorsInWorld();
		for (auto actor : actors)
		{
			if (dynamic_cast<T*>(actor))
			{
				outActors.emplace_back(actor);
			}
		}

		return outActors;
	}

	template <typename T>
	std::vector<T*> GetAllActorsAsBaseType()
	{
		std::vector<T*> outActors;
		for (auto actor : World::GetAllActorsInWorld())
		{
			auto actorType = dynamic_cast<T*>(actor);
			if (actorType)
			{
				outActors.push_back(actorType);
			}
		}
		return outActors;
	}

	template <typename T>
	void RemoveAllActorsOfType()
	{
		auto actors = GetAllActorsInWorld();
		for (auto actor : actors)
		{
			if (dynamic_cast<T*>(actor))
			{
				auto actorSystem = actor->GetActorSystem();
				actorSystem->RemoveInterfaceActor(actor);
			}
		}
	}
};
