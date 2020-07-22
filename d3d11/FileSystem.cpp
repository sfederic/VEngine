#include "FileSystem.h"
#include "Actor.h"
#include "World.h"
#include "DebugMenu.h"
#include "Input.h"
#include "Debug.h"

FileSystem g_FileSystem;

void FileSystem::Tick()
{
	//Test world load file handling.
	if (inputSystem.GetKeyUpState('4'))
	{
		g_FileSystem.WriteAllActorSystems(GetWorld(), "LevelSaves/test.sav");
	}

	if (inputSystem.GetKeyUpState('5'))
	{
		g_FileSystem.ReadAllActorSystems(GetWorld(), "LevelSaves/test.sav");
	}
}

void FileSystem::WriteAllActorSystems(World* world, const char* filename)
{
	//TODO: make filename work with current World name
	fopen_s(&file, filename, "w");
	assert(file);

	int numActorSystems = world->actorSystems.size();
	fwrite(&numActorSystems, sizeof(int), 1, file);

	for (int systemIndex = 0; systemIndex < world->actorSystems.size(); systemIndex++)
	{
		fwrite(&world->actorSystems[systemIndex].id, sizeof(int), 1, file);

		int numOfActors = world->actorSystems[systemIndex].actors.size();
		fwrite(&numOfActors, sizeof(int), 1, file);

		for (int actorIndex = 0; actorIndex < world->actorSystems[systemIndex].actors.size(); actorIndex++)
		{
			//fwrite without the for loop(SOA) was about 0.01 ms faster with around 60,000 actors. Surprising
			fwrite(&world->actorSystems[systemIndex].actors[actorIndex].transform, sizeof(XMMATRIX), 1, file);
		}
	}

	DebugPrint("All actor systems saved.\n");
	debugMenu.notifications.push_back(DebugNotification(L"All actor systems saved"));

	fclose(file);
}

void FileSystem::ReadAllActorSystems(World* world, const char* filename)
{
	fopen_s(&file, filename, "r");
	assert(file);

	World newWorld = {};
	int numActorSystems = 0;
	fread(&numActorSystems, sizeof(int), 1, file);

	for (int systemIndex = 0; systemIndex < numActorSystems; systemIndex++)
	{
		newWorld.actorSystems.push_back(ActorSystem());

		fread(&newWorld.actorSystems[systemIndex].id, sizeof(int), 1, file);

		int numActorsToSpawn = 0;
		fread(&numActorsToSpawn, sizeof(int), 1, file);

		switch (newWorld.actorSystems[systemIndex].id)
		{
		case EActorSystemID::Base:
			newWorld.actorSystems[systemIndex].CreateActors(&renderSystem, numActorsToSpawn);
		}

		for (int actorIndex = 0; actorIndex < world->actorSystems[systemIndex].actors.size(); actorIndex++)
		{
			fread(&newWorld.actorSystems[systemIndex].actors[actorIndex].transform, sizeof(XMMATRIX), 1, file);
		}

	}

	currentWorld.CleaupAllActors();

	currentWorld = newWorld;

	DebugPrint("All actor systems loaded.\n");
	debugMenu.notifications.push_back(DebugNotification(L"All actor systems loaded."));

	fclose(file);
}