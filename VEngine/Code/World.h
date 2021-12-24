#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "UID.h"

struct IActorSystem;
struct IComponentSystem;
struct Component;
struct Actor;

struct World
{
	std::vector<IActorSystem*> activeActorSystems;
	std::vector<IComponentSystem*> activeComponentSystems;

	std::unordered_map<UID, Actor*> actorUIDMap;
	std::unordered_map<std::string, Actor*> actorNameMap;

	std::string worldFilename;

	//Called at engine startup
	void Init();

	//Called on level load
	void Start();

	//Called when gameplay begins
	void WakeAndStartAllActors();
	void StartAllComponents();

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

	Actor* GetActorByUID(UID uid);
	Actor* GetActorByName(std::string actorName);
	Actor* GetActorByNameAllowNull(std::string actorName);
	std::vector<Component*> GetAllComponentsInWorld();
	void Cleanup();
};

extern World world;
