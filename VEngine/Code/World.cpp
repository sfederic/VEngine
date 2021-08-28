#include "World.h"
#include "Actors/NormalActor.h"
#include "Actors/MeshActor.h"
#include "Actors/ActorSystemCache.h"
#include "Components/ComponentSystemCache.h"
#include "Components/IComponentSystem.h"
#include "Components/MeshComponent.h"

World world;

void World::Start()
{
	//TODO: there's an optimization you could do here where you only add in systems that have at least
	//one element instead of grabbing them all from their respective cache. Problem is you'd have to check
	//through systems in the world and check how many elements are in their vectors. Might not be worth it for
	//small projects.

	//Add actorsystems into world
	for (auto actorSystemIt : *actorSystemCache.actorSystemMap)
	{
		activeActorSystems.push_back(actorSystemIt.second);
	}

	//Add componentsystems into world
	for (auto componentSystemIt : *componentSystemCache.componentSystemMap)
	{
		activeComponentSystems.push_back(componentSystemIt.second);
	}
}

void World::TickAllActorSystems(double deltaTime)
{
	for (IActorSystem* actorSystem : activeActorSystems)
	{
		actorSystem->Tick(deltaTime);
	}
}

void World::TickAllComponentSystems(double deltaTime)
{
	for (IComponentSystem* componentSystem : activeComponentSystems)
	{
		componentSystem->Tick(deltaTime);
	}
}

Actor* World::FindActorByName(std::string actorName)
{
	for (IActorSystem* actorSystem : activeActorSystems)
	{
		Actor* foundActor = actorSystem->FindActorByName(actorName);

		if (foundActor)
		{
			return foundActor;
		}
	}

	return nullptr;
}

void World::Cleanup()
{
	//CLEANUP COMPONENT SYSTEMS
	for (IComponentSystem* componentSystem : activeComponentSystems)
	{
		componentSystem->Cleanup();
	}

	//CLEANUP ACTOR SYSTEMS
	for (IActorSystem* actorSystem : activeActorSystems)
	{
		actorSystem->Cleanup();
	}

	activeActorSystems.clear();
}
