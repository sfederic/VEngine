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

	Serialiser s(file, std::ios_base::out);
	std::ostream os(&s.fb);

	for (IActorSystem* actorSystem : world.activeActorSystems)
	{
		actorSystem->Serialise(os);
	}
}

void FileSystem::LoadWorld(std::string worldName)
{
	world.CleanupActorSystems();

	std::string path = "WorldMaps/" + worldName;

	Serialiser s(path, std::ios_base::in);
	std::istream is(&s.fb);

	while (!is.eof())
	{
		std::string actorSystemName;
		is >> actorSystemName;

		if (actorSystemName == "")
		{
			break;
		}

		size_t numActorsToSpawn = 0;
		is >> numActorsToSpawn;

		auto asIt = actorSystemCache.nameToSystemMap->find(actorSystemName);

		IActorSystem* actorSystem = asIt->second;
		world.activeActorSystems.push_back(actorSystem);

		actorSystem->name.assign(actorSystemName);

		for (int i = 0; i < numActorsToSpawn; i++)
		{
			actorSystem->SpawnActor(Transform());
		}

		actorSystem->Deserialise(is);
	}

	//Deselect any existing actors, because TransformGizmo will stay at previous positions.
	worldEditor.pickedActor = nullptr;
	editor->UpdateWorldList();
	debugMenu.AddNotification(L"Level loaded.");
}
