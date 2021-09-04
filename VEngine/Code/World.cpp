#include "World.h"
#include "Actors/NormalActor.h"
#include "Actors/MeshActor.h"
#include "Actors/ActorSystemCache.h"
#include "Components/ComponentSystemCache.h"
#include "Components/IComponentSystem.h"
#include "Components/MeshComponent.h"

#include "Profile.h"

World world;

void World::Init()
{
	//Add actorsystems into world
	for (auto actorSystemIt : *actorSystemCache.typeToSystemMap)
	{
		activeActorSystems.push_back(actorSystemIt.second);
	}

	//Add componentsystems into world
	for (auto componentSystemIt : *componentSystemCache.typeToSystemMap)
	{
		activeComponentSystems.push_back(componentSystemIt.second);
	}

	//Start default loaded world
	//Start();
}

void World::Start()
{
	//Init component systems
	for (IComponentSystem* componentSystem : activeComponentSystems)
	{
		componentSystem->Init();
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

Actor* World::FindComponentOwnerByName(std::string componentName)
{
	for (IActorSystem* actorSystem : activeActorSystems)
	{
		std::vector<Actor*> actors;
		actorSystem->GetActors(actors);

		for (Actor* actor : actors)
		{
			for (Component* component : actor->components)
			{
				if (component->name == componentName)
				{
					return actor;
				}
			}
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
}
