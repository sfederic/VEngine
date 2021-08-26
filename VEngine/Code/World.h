#pragma once
#include <vector>
#include <string>

struct IActorSystem;
struct IComponentSystem;
struct Actor;

struct World
{
	std::vector<IActorSystem*> activeActorSystems;
	std::vector<IComponentSystem*> activeComponentSystems;

	void Start();
	void TickAllActorSystems(double deltaTime);
	void TickAllComponentSystems(double deltaTime);
	Actor* FindActorByName(std::string actorName);
	void Cleanup();
};

extern World world;
