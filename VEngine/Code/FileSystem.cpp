#include "FileSystem.h"
#include "World.h"
#include "Serialiser.h"
#include "Actors/IActorSystem.h"
#include "Actors/ActorSystemCache.h"
#include "WorldEditor.h"
#include "Editor/DebugMenu.h"
#include "Editor/Editor.h"

FileSystem fileSystem;

void FileSystem::WriteAllActorSystems(std::string worldName)
{
	std::string file = "WorldMaps/" + worldName;
	file += ".sav";

	Serialiser s(file, OpenMode::Out);

	for (IActorSystem* actorSystem : world.activeActorSystems)
	{
		actorSystem->Serialise(s);
	}

	debugMenu.AddNotification(L"World saved.");
}

void FileSystem::LoadWorld(std::string worldName)
{
	world.Cleanup();

	Serialiser s(worldName, OpenMode::In);

	while (!s.is.eof())
	{
		std::string actorSystemName;
		s.is >> actorSystemName;

		if (actorSystemName == "")
		{
			break;
		}

		size_t numActorsToSpawn = 0;
		s.is >> numActorsToSpawn;

		auto asIt = actorSystemCache.nameToSystemMap->find(actorSystemName);

		IActorSystem* actorSystem = asIt->second;

		for (int i = 0; i < numActorsToSpawn; i++)
		{
			actorSystem->SpawnActor(Transform());
		}

		actorSystem->Deserialise(s);
	}

	//Re-init world
	world.Start();

	//Deselect any existing actors, because TransformGizmo will stay at previous positions.
	worldEditor.pickedActor = nullptr;

	editor->UpdateWorldList();
	debugMenu.AddNotification(L"World loaded.");
}
