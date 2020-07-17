#include "FileSystem.h"
#include "Actor.h"
#include "World.h"
#include "DebugMenu.h"

void FileSystem::WriteAllActorSystems(World* world, const char* filename)
{
	//TODO: make filename work with current World name
	fopen_s(&file, filename, "w");
	assert(file);

	for (int systemIndex = 0; systemIndex < world->actorSystems.size(); systemIndex++)
	{
		for (int actorIndex = 0; actorIndex < world->actorSystems[systemIndex]->actors.size(); actorIndex++)
		{
			//fwrite without the for loop(SOA) was about 0.01 ms faster with around 60,000 actors. Surprising
			fwrite(&world->actorSystems[systemIndex]->actors[actorIndex].transform, sizeof(XMMATRIX), 1, file);
		}
	}

	DebugPrint("All actor systems saved.\n");
	g_DebugMenu.notifications.push_back(DebugNotification(L"All actor systems saved"));

	fclose(file);
}

void FileSystem::ReadAllActorSystems(World* world, const char* filename)
{
	fopen_s(&file, filename, "r");
	assert(file);

	for (int systemIndex = 0; systemIndex < world->actorSystems.size(); systemIndex++)
	{
		for (int actorIndex = 0; actorIndex < world->actorSystems[systemIndex]->actors.size(); actorIndex++)
		{
			fread(&world->actorSystems[systemIndex]->actors[actorIndex].transform, sizeof(XMMATRIX), 1, file);
		}
	}

	DebugPrint("All actor systems loaded.\n");
	g_DebugMenu.notifications.push_back(DebugNotification(L"All actor systems loaded."));

	fclose(file);
}