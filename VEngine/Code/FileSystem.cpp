#include "FileSystem.h"
#include "World.h"
#include "Serialiser.h"
#include "Actors/IActorSystem.h"
#include "Actors/ActorSystemCache.h"
#include "WorldEditor.h"
#include "Editor/DebugMenu.h"
#include "Editor/Editor.h"
#include "Commands/CommandSystem.h"

FileSystem fileSystem;

void FileSystem::WriteAllActorSystems()
{
	auto lastOf = world.worldFilename.find_last_of("/\\");
	std::string str = world.worldFilename.substr(lastOf + 1);
	std::string file = "WorldMaps/" + str;

	Serialiser s(file, OpenMode::Out);

	for (IActorSystem* actorSystem : world.activeActorSystems)
	{
		actorSystem->Serialise(s);
	}

	debugMenu.AddNotification(L"World saved.");
}

void FileSystem::LoadWorld(std::string worldName)
{
	world.worldFilename = worldName;

	world.Cleanup();

	//Re-init world
	//world.Start();

	Serialiser s(worldName, OpenMode::In);

	while (!s.is.eof())
	{
		std::string actorSystemName;
		s.is >> actorSystemName;

		if (actorSystemName.empty())
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

	world.Start();

	//Deselect any existing actors, because TransformGizmo will stay at previous positions.
	worldEditor.pickedActor = nullptr;

	//Clear all commands
	commandSystem.commands.clear();

	editor->UpdateWorldList();
	debugMenu.AddNotification(L"World loaded.");
}
