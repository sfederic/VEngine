#include "World.h"
#include "Actor.h"
#include "DebugMenu.h"
#include "CoreSystem.h"
#include "EditorSystem.h"
#include "Profiler.h"

World gCurrentWorld;

//Returns the current active world
World* GetWorld()
{
	return &gCurrentWorld;
}

void World::Load(std::string levelName)
{
	FILE* file;
	fopen_s(&file, levelName.c_str(), "rb");
	assert(file);

	fclose(file);
}

void World::TickAllActorSystems(float deltaTime)
{
	PROFILE_START

	//Skip ticks if game is paused
	if (gCoreSystem.bGamePaused)
	{
		//return;
	}

	//Skip ticks if game in-editor isn't running
	if (!gCoreSystem.bGamePlayOn)
	{
		//return;
	}

	for(ActorSystem* actorSystem : actorSystems)
	{
		actorSystem->Tick(deltaTime);

		for (Actor* actor : actorSystem->actors)
		{
			actor->Tick(deltaTime);

			if (actor->parent == nullptr)
			{
				actor->UpdateTransform(XMMatrixIdentity());
			}
		}
	}

	PROFILE_END
}

void World::StartAllActorSystems()
{
	for (auto& as : actorSystems)
	{
		as->Start();

		for (auto& actor : as->actors)
		{
			actor->Start();
		}
	}
}

void World::CleaupAllActors()
{
	actorSystems.clear();
	actorSystemsMap.clear();
}

void World::AddActorSystem(ActorSystem* actorSystem)
{
	auto actorSystemIt = actorSystemsMap.find(actorSystem->name);
	if (actorSystemIt == actorSystemsMap.end())
	{
		actorSystem->SpawnActors(1); //Spawn one dummy actor so that buffers set up properly
		actorSystems.push_back(actorSystem);
		actorSystemsMap.insert(std::pair(actorSystem->name, actorSystem));

		auto asIt = ActorSystemFactory::nameToSystemMap->find(actorSystem->name);
		if (asIt == ActorSystemFactory::nameToSystemMap->end())
		{
			//This is to spawn template actorsystems that won't exist in the factory
			ActorSystemFactory::Register<ActorSystem>(actorSystem);
		}
	}

	gEditorSystem->PopulateActorSystemList();
	gEditorSystem->PopulateWorldList();
}

Actor* World::FindActorByString(const std::string& name)
{
	Actor* foundActor = nullptr;

	for (int i = 0; i < actorSystems.size(); i++)
	{
		for (int j = 0; j < actorSystems[i]->actors.size(); j++)
		{
			if (actorSystems[i]->actors[j]->name == name)
			{
				foundActor = actorSystems[i]->GetActor(j);
				return foundActor;
			}
		}
	}

	return foundActor;
}

ActorSystem* World::GetActorSystem(unsigned int id)
{
	if (id < actorSystems.size())
	{
		return actorSystems[id];
	}

	return nullptr;
}

Actor* World::GetActor(unsigned int systemId, unsigned int actorId)
{
	if (systemId < actorSystems.size())
	{
		if (actorId < actorSystems[systemId]->actors.size())
		{
			return actorSystems[systemId]->actors[actorId];
		}
	}

	return nullptr;
}

uint64_t World::GetNumOfActorsInWorld()
{
	uint64_t numOfActorsInWorld = 0;

	for (auto& actorSystem : actorSystems)
	{
		numOfActorsInWorld += actorSystem->actors.size();
	}

	return numOfActorsInWorld;
}
