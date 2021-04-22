#include "FileSystem.h"
#include "Actor.h"
#include "World.h"
#include "DebugMenu.h"
#include "Input.h"
#include "Debug.h"
#include "UISystem.h"
#include "RenderSystem.h"

FileSystem gFileSystem;

void FileSystem::Tick()
{
	//world load file handling.
	if (gInputSystem.GetKeyUpState(VK_F4))
	{
		gFileSystem.WriteAllActorSystems(GetWorld(), "LevelSaves/test.sav");
	}

	if (gInputSystem.GetKeyUpState(VK_F5))
	{
		gFileSystem.ReadAllActorSystems(GetWorld(), "LevelSaves/test.sav");
	}
}

void FileSystem::WriteAllActorSystems(World* world, const char* filename)
{
	//TODO: make filename work with current World name
	fopen_s(&file, filename, "w");
	assert(file);

	for (int systemIndex = 0; systemIndex < world->actorSystems.size(); systemIndex++)
	{
		int numOfActors = world->actorSystems[systemIndex]->actors.size();
		fwrite(&numOfActors, sizeof(int), 1, file);

		for (int actorIndex = 0; actorIndex < world->actorSystems[systemIndex]->actors.size(); actorIndex++)
		{
			//fwrite without the for loop(SOA) was about 0.01 ms faster with around 60,000 actors. Surprising.
			fwrite(&world->actorSystems[systemIndex]->actors[actorIndex]->transform,
				sizeof(Transform), 1, file);
		}
	}

	DebugPrint("All actor systems saved.\n");
	gDebugMenu.notifications.push_back(DebugNotification(L"All actor systems saved"));

	fclose(file);
}

void FileSystem::ReadAllActorSystems(World* world, const char* filename)
{
	fopen_s(&file, filename, "r");
	assert(file);

	for (int systemIndex = 0; systemIndex < world->actorSystems.size(); systemIndex++)
	{
		int numActors = 0;
		fread(&numActors, sizeof(int), 1, file);

		for (int i = 0; i < numActors; i++)
		{
			fread(&world->actorSystems[systemIndex]->actors[i]->transform,
				sizeof(Transform), 1, file);
		}
	}

	DebugPrint("All actor systems loaded.\n");
	gDebugMenu.notifications.push_back(DebugNotification(L"All actor systems loaded."));

	fclose(file);
}