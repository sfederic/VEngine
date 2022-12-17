#pragma once
#include <vector>
#include <string>
#include "UID.h"

class IActorSystem;
struct IComponentSystem;
struct Component;
class Actor;

namespace World
{
	extern std::string worldFilename;

	extern std::unordered_map<UID, Actor*> actorUIDMap;
	extern std::unordered_map<std::string, Actor*> actorNameMap;

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

	//Create default starting actors for a map.
	void CreateDefaultMapActors();

	void CreateNodeGrid();

	std::vector<IActorSystem*> GetLayerActorSystems();

	void TickAllActorSystems(float deltaTime);
	void TickAllComponentSystems(float deltaTime);
	std::vector<Actor*> GetAllActorsInWorld();

	template <typename T>
	std::vector<T*> GetAllActorsOfTypeInWorld()
	{
		std::vector<T*> outActors;

		auto actors = GetAllActorsInWorld();
		for (auto actor : actors)
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
	std::vector<T*> GetAllComponentSystemsOfType()
	{
		std::vector<T*> outComponentSystems;

		for (auto componentSystem : activeComponentSystems)
		{
			auto systemType = dynamic_cast<T*>(componentSystem);
			if (systemType)
			{
				outComponentSystems.push_back(systemType);
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
				outActors.push_back(actor);
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
};
