#pragma once
#include <vector>
#include <string>

struct IActorSystem;
struct IComponentSystem;
struct Component;
struct Actor;

struct World
{
	std::vector<IActorSystem*> activeActorSystems;
	std::vector<IComponentSystem*> activeComponentSystems;

	std::string worldFilename;

	//Called at engine startup
	void Init();

	//Called on level load
	void Start();

	//Called when gameplay begins
	void StartAllActors();
	void StartAllComponents();

	void TickAllActorSystems(float deltaTime);
	void TickAllComponentSystems(float deltaTime);
	Actor* FindActorByName(std::string actorName);
	Actor* FindComponentOwnerByName(std::string componentName);
	std::vector<Actor*> GetAllActorsInWorld();
	std::vector<Component*> GetAllComponentsInWorld();
	void Cleanup();
};

extern World world;
