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
#include "Core.h"
#include "Audio/AudioSystem.h"

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
	fileSystem.LoadWorld("test2.vmap");

	//Start default loaded world
	//Start();
}

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

	if (Core::gameplayOn)
	{
		world.StartAllActors();
		world.StartAllComponents();
	}
}

void World::StartAllActors()
{
	auto actors = GetAllActorsInWorld();

	for (auto actor : actors)
	{
		actor->Start();
	}
}

void World::StartAllComponents()
{
	auto components = GetAllComponentsInWorld();

	for (auto component : components)
	{
		component->Start();
	}
}

std::vector<IActorSystem*> World::GetLayerActorSystems()
{
	std::vector<IActorSystem*> actorSystems;

	for (auto actorSystem : activeActorSystems)
	{
		if (actorSystem->name == "EntranceTrigger" ||
			actorSystem->name == "MeshActor")
		{
			continue;
		}

		actorSystems.push_back(actorSystem);
	}

	return actorSystems;
}

void World::TickAllActorSystems(float deltaTime)
{
	PROFILE_START

	for (IActorSystem* actorSystem : activeActorSystems)
	{
		actorSystem->Tick(deltaTime);
	}

	PROFILE_END
}

void World::TickAllComponentSystems(float deltaTime)
{
	PROFILE_START

	for (IComponentSystem* componentSystem : activeComponentSystems)
	{
		componentSystem->Tick(deltaTime);
	}

	PROFILE_END
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

Actor* World::GetActorByUID(UID uid)
{
	auto actorIt = actorUIDMap.find(uid);
	assert(actorIt != actorUIDMap.end());
	return actorIt->second;
}

Actor* World::GetActorByName(std::string actorName)
{
	auto actorIt = actorNameMap.find(actorName);
	if (actorIt == actorNameMap.end())
	{
		return nullptr;
	}
	return actorIt->second;
}

std::vector<Component*> World::GetAllComponentsInWorld()
{
	std::vector<Component*> outComponents;

	for (IComponentSystem* componentSystem : activeComponentSystems)
	{
		auto components = componentSystem->GetComponents();
		outComponents.insert(outComponents.end(), components.begin(), components.end());
	}

	return outComponents;
}

void World::Cleanup()
{
	actorUIDMap.clear();
	actorNameMap.clear();

	//Cleanup various systems
	audioSystem.DeleteLoadedAudioAndChannels();
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
