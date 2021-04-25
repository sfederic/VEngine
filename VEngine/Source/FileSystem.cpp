#include "FileSystem.h"
#include "Actor.h"
#include "World.h"
#include "DebugMenu.h"
#include "Input.h"
#include "Debug.h"
#include "UISystem.h"
#include "RenderSystem.h"
#include "Actors/TestActor.h"
#include "ActorSystemFactory.h"

FileSystem gFileSystem;

void FileSystem::Tick()
{
	//actorsystem save/load input
	if (gInputSystem.GetKeyUpState(VK_F4))
	{
		gFileSystem.WriteAllActorSystems(GetWorld(), "LevelSaves/test.sav");
	}
	if (gInputSystem.GetKeyUpState(VK_F5))
	{
		gFileSystem.ReloadAllActorSystems(GetWorld(), "LevelSaves/test.sav");
	}
}

//This is a 'ground-up' version of ReloadAllActorSystems() where the systems will create their actors
//on load and then load all the actor data in.
void FileSystem::LoadWorld(const char* levelName)
{
	fopen_s(&file, levelName, "rb");
	assert(file);

	World* world = GetWorld();
	world->CleaupAllActors();

	uint64_t numActorSystemsToLoad = 0;
	fread(&numActorSystemsToLoad, sizeof(numActorSystemsToLoad), 1, file);

	for (int systemIndex = 0; systemIndex < numActorSystemsToLoad; systemIndex++)
	{
		uint64_t actorSystemID = 0;
		fread(&actorSystemID, sizeof(uint64_t), 1, file);

		ActorSystem* actorSystem = ActorSystemFactory::GetActorSystem(actorSystemID);
		world->AddActorSystem(actorSystem);

		uint64_t numActorsToLoad = 0;
		fread(&numActorsToLoad, sizeof(uint64_t), 1, file);

		actorSystem->CreateActors<TestActor>(&gRenderSystem, numActorsToLoad);

		for (int actorIndex = 0; actorIndex < numActorsToLoad; actorIndex++)
		{
			fread(actorSystem->actors[actorIndex], actorSystem->sizeofActor, 1, file);
		}
	}
}

void FileSystem::WriteAllActorSystems(World* world, const char* filename)
{
	fopen_s(&file, filename, "wb");
	assert(file);

	uint64_t numActorSystemsToSave = world->actorSystems.size();
	fwrite(&numActorSystemsToSave, sizeof(uint64_t), 1, file);

	for (int systemIndex = 0; systemIndex < world->actorSystems.size(); systemIndex++)
	{
		ActorSystem* actorSystem = world->actorSystems[systemIndex];

		uint64_t actorSystemID = ActorSystemFactory::GetActorSystemID(actorSystem);
		fwrite(&actorSystemID, sizeof(uint64_t), 1, file);

		uint64_t numOfActors = actorSystem->actors.size();
		fwrite(&numOfActors, sizeof(uint64_t), 1, file);

		for (int actorIndex = 0; actorIndex < world->actorSystems[systemIndex]->actors.size(); actorIndex++)
		{
			//fwrite without the for loop(SOA) was about 0.01 ms faster with around 60,000 actors. Surprising.
			fwrite(actorSystem->actors[actorIndex], actorSystem->sizeofActor, 1, file);
		}
	}

	DebugPrint("All actor systems saved.\n");
	gDebugMenu.notifications.push_back(DebugNotification(L"All actor systems saved"));

	fclose(file);
}

//This function works on like a 'hot-reload' basis when the level is already open
//and actor systems are persistent. Doesn't work with level loading/streaming.
void FileSystem::ReloadAllActorSystems(World* world, const char* filename)
{
	fopen_s(&file, filename, "rb");
	assert(file);

	for (int systemIndex = 0; systemIndex < world->actorSystems.size(); systemIndex++)
	{
		ActorSystem* actorSystem = world->actorSystems[systemIndex];

		uint64_t actorSystemID;
		fread(&actorSystemID, sizeof(uint64_t), 1, file);

		uint64_t numActors = 0;
		fread(&numActors, sizeof(uint64_t), 1, file);

		for (int i = 0; i < numActors; i++)
		{
			fread(world->actorSystems[systemIndex]->actors[i], actorSystem->sizeofActor, 1, file);
		}
	}

	DebugPrint("All actor systems loaded.\n");
	gDebugMenu.notifications.push_back(DebugNotification(L"All actor systems loaded."));

	fclose(file);
}
