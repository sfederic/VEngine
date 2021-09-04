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
	void TickAllActorSystems(double deltaTime);
	void TickAllComponentSystems(double deltaTime);
	Actor* FindActorByName(std::string actorName);
	Actor* FindComponentOwnerByName(std::string componentName);
	void Cleanup();
};

extern World world;
