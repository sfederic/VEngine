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
#include "WorldEditor.h"
#include "EditorSystem.h"

FileSystem gFileSystem;

void FileSystem::Tick()
{
	//actorsystem save/load input
	if (gInputSystem.GetAsyncKey(Keys::Ctrl))
	{
		if (gInputSystem.GetKeyDownState(Keys::S))
		{
			gFileSystem.WriteAllActorSystems(GetWorld(), "test");
		}
	}
}

void FileSystem::LoadWorld(const std::string& levelName)
{
	World* world = GetWorld();
	world->CleaupAllActors();

	std::string path = levelName;

	Serialiser s(path, std::ios_base::in);
	std::istream is(&s.fb);

	while (!is.eof())
	{
		std::string actorSystemName;
		is >> actorSystemName;
		auto asIt = ActorSystemFactory::nameToSystemMap->find(actorSystemName);
		if (asIt == ActorSystemFactory::nameToSystemMap->end())
		{
			assert(true && "actorsystem not found in factory");
		}

		size_t numActorsToSpawn = 0;
		is >> numActorsToSpawn;

		ActorSystem* actorSystem = asIt->second;
		actorSystem->Cleanup();

		world->AddActorSystem(actorSystem);

		actorSystem->SpawnActors(numActorsToSpawn);
		actorSystem->Deserialise(is);
	}
	 
	//Deselect any existing actors, because TransformGizmo will stay at previous positions.
	gWorldEditor.pickedActor = nullptr;

	gEditorSystem->PopulateWorldList();

	gDebugMenu.AddNotification(L"Level loaded.");
}

void FileSystem::WriteAllActorSystems(World* world, const std::string& levelName)
{
	std::string file = "LevelSaves/" + levelName;
	file += ".sav"; //.sav files are still .txt files

	Serialiser s(file, std::ios_base::out);
	std::ostream os(&s.fb);

	for (ActorSystem* actorSystem : world->actorSystems)
	{
		actorSystem->Serialise(os);
	}

	DebugPrint("All actor systems saved.\n");
	gDebugMenu.AddNotification(L"All actor systems saved");
}
