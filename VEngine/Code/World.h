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
	void StartAllActors();
	void StartAllComponents();

	std::vector<IActorSystem*> GetLayerActorSystems();

	void TickAllActorSystems(float deltaTime);
	void TickAllComponentSystems(float deltaTime);
	Actor* FindActorByName(std::string actorName);
	Actor* FindComponentOwnerByName(std::string componentName);
	std::vector<Actor*> GetAllActorsInWorld();
	Actor* GetActor(UID uid);
	Actor* GetActor(std::string actorName);
	std::vector<Component*> GetAllComponentsInWorld();
	void Cleanup();
};

extern World world;
