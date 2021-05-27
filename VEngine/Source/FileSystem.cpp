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
			gFileSystem.WriteAllActorSystems(GetWorld());
		}
	}
}

void FileSystem::LoadWorld(const std::string& levelName)
{
	World* world = GetWorld();
	world->CleaupAllActors();

	for (ActorSystem* actorSystem : world->actorSystems)
	{
		actorSystem->Deserialise(levelName);
	}

	//world->AddActorSystem(actorSystem);
	//actorSystem->SpawnActors(numActorsToLoad);
	 
	//Deselect any existing actors, because TransformGizmo will stay at previous positions.
	//gWorldEditor.pickedActor = nullptr;

	gDebugMenu.AddNotification(L"Level loaded.");

	//gEditorSystem->PopulateWorldList();
}

void FileSystem::WriteAllActorSystems(World* world, const std::string& levelName)
{
	for (ActorSystem* actorSystem : world->actorSystems)
	{
		actorSystem->Serialise(levelName);
	}

	DebugPrint("All actor systems saved.\n");
	gDebugMenu.AddNotification(L"All actor systems saved");
}
