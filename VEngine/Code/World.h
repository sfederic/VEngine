#pragma once
#include <vector>

struct IActorSystem;
struct IComponentSystem;

struct World
{
	std::vector<IActorSystem*> activeActorSystems;
	std::vector<IComponentSystem*> activeComponentSystems;

	void Start();
};

extern World world;
