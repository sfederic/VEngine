#include "World.h"
#include "Actors/NormalActor.h"
#include "Components/IComponentSystem.h"
#include "Components/MeshComponent.h"

World world;

void World::Start()
{
	activeActorSystems.push_back(&NormalActor::system);
	activeComponentSystems.push_back(&MeshComponent::system);
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
