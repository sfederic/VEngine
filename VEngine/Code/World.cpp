#include "World.h"
#include "Actors/MeshActor.h"
#include "Actors/ActorSystemCache.h"
#include "Components/ComponentSystemCache.h"
#include "Components/IComponentSystem.h"
#include "Components/MeshComponent.h"
#include "FileSystem.h"
#include "Profile.h"
#include "Render/TextureSystem.h"
#include "Render/MaterialSystem.h"

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

	//Load starting map
	fileSystem.LoadWorld("WorldMaps/test2.sav");

	//Start default loaded world
	//Start();
}

//TODO: this sort of stuff doesn't work when deserialisation is onvolved as is.
void World::Start()
{
	materialSystem.CreateAllMaterials();
	textureSystem.CreateAllTextures();

	//Init actor systems
	for (IActorSystem* actorSystem : activeActorSystems)
	{
		actorSystem->Init();
	}

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
	for (Actor* actor : GetAllActorsInWorld())
	{
		for (Component* component : actor->components)
		{
			if (component->name == componentName)
			{
				return actor;
			}
		}
	}

	return nullptr;
}

std::vector<Actor*> World::GetAllActorsInWorld()
{
	std::vector<Actor*> outActors;

	for (IActorSystem* actorSystem : activeActorSystems)
	{
		auto actors = actorSystem->GetActors();
		outActors.insert(outActors.end(), actors.begin(), actors.end());
	}

	return outActors;
}

void World::Cleanup()
{
	textureSystem.Cleanup();
	materialSystem.Cleanup();

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
