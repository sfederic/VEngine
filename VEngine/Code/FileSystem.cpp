#include "vpch.h"
#include "FileSystem.h"
#include <filesystem>
#include "World.h"
#include "Serialiser.h"
#include "Actors/IActorSystem.h"
#include "Actors/ActorSystemCache.h"
#include "WorldEditor.h"
#include "Editor/DebugMenu.h"
#include "Editor/Editor.h"
#include "Commands/CommandSystem.h"
#include "Core.h"
#include "Gameplay/GameUtils.h"
#include "Camera.h"
#include "Actors/Game/Player.h"
#include "VString.h"
#include "Input.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/BattleSystem.h"
#include "Quests/QuestSystem.h"

std::pair<UID, std::string> GetComponentOwnerUIDAndNameOnDeserialise(Deserialiser& d)
{
	UID ownerUID = 0;
	d.is >> ownerUID;

	std::wstring componentWName;
	d.is >> componentWName;
	std::string componentName = VString::wstos(componentWName);

	return std::pair<UID, std::string>(ownerUID, componentName);
}

void FileSystem::SerialiseAllSystems()
{
	auto lastOf = world.worldFilename.find_last_of("/\\");
	std::string str = world.worldFilename.substr(lastOf + 1);

	std::string file = "WorldMaps/" + str;

	if (GameInstance::useGameSaves)
	{
		file = "GameSaves/" + str;
	}

	Serialiser s(file, OpenMode::Out);

	for (auto actorSystem : world.activeActorSystems)
	{
		actorSystem->Serialise(s);
	}

	for (auto cs : world.activeComponentSystems)
	{
		cs->Serialise(s);
	}

	s.WriteLine(L"end");

	debugMenu.AddNotification(VString::wformat(L"%S world saved", world.worldFilename.c_str()));
}

void FileSystem::WriteAllActorSystemsToBinary()
{
	auto lastOf = world.worldFilename.find_last_of("/\\");
	std::string str = world.worldFilename.substr(lastOf + 1);

	std::string file = "WorldMaps/Binary/" + str;

	BinarySerialiser s(file);

	for (IActorSystem* actorSystem : world.activeActorSystems)
	{
		actorSystem->SerialiseBinary(s);
	}

	debugMenu.AddNotification(VString::wformat(L"%S world saved to binary", world.worldFilename.c_str()));
}

//@Todo: binary serialisation isn't working too well. Might be some weird alignment errors taken from property sizes.
void FileSystem::ReadAllActorSystemsFromBinary()
{
	std::string worldName = world.worldFilename;

	std::string path = "WorldMaps/Binary/" + worldName;

	assert(std::filesystem::exists(path) && "Map file doesn't exist");

	world.Cleanup();

	BinaryDeserialiser d(path);

	while (!feof(d.file))
	{
		size_t stringSize = 0;
		auto res = fread(&stringSize, sizeof(size_t), 1, d.file);
		if (res == 0)
		{
			break;
		}

		std::string actorSystemName;
		res = fread(actorSystemName.data(), sizeof(char), stringSize, d.file);
		if (res == 0)
		{
			break;
		}

		int numActorsToSpawn = 0;
		res = fread(&numActorsToSpawn, sizeof(int), 1, d.file);
		if (res == 0)
		{
			break;
		}

		auto asIt = actorSystemCache.nameToSystemMap->find(actorSystemName.data());
		if (asIt == actorSystemCache.nameToSystemMap->end())
		{
			continue;
		}

		auto actorSystem = asIt->second;

		for (int i = 0; i < numActorsToSpawn; i++)
		{
			Actor* actor = actorSystem->SpawnActor(Transform());
		}

		actorSystem->DeserialiseBinary(d);
	}

	ResetWorldState();

	debugMenu.AddNotification(VString::wformat(L"%S world loaded from binary", world.worldFilename.c_str()));
}

void FileSystem::LoadWorld(std::string worldName)
{
	editor->SetEditorTitle(worldName);

	world.worldFilename = worldName;

	std::string path = "WorldMaps/" + worldName;

	if (GameInstance::useGameSaves)
	{
		path = "GameSaves/" + worldName;
	}

	assert(std::filesystem::exists(path) && "Map file doesn't exist");

	world.Cleanup();

	Deserialiser d(path, OpenMode::In);

	while (!d.is.eof())
	{
		std::wstring actorSystemName;
		d.is >> actorSystemName;

		if (actorSystemName == L"end")
		{
			break;
		}

		if (actorSystemName.empty())
		{
			d.is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}

		size_t numObjectsToSpawn = 0;
		d.is >> numObjectsToSpawn;
		if (numObjectsToSpawn == 0)
		{
			continue;
		}

		auto asIt = actorSystemCache.nameToSystemMap->find(VString::wstos(actorSystemName));
		if (asIt == actorSystemCache.nameToSystemMap->end())
		{
			auto csIt = componentSystemCache.nameToSystemMap->find(VString::wstos(actorSystemName));
			if (csIt == componentSystemCache.nameToSystemMap->end())
			{
				continue;
			}
			else
			{
				auto cs = csIt->second;

				//Deserialise the existing components created in Actor constructors
				for (int i = 0; i < cs->GetNumComponents(); i++)
				{
					auto ownerUIDAndName = GetComponentOwnerUIDAndNameOnDeserialise(d);

					Actor* owner = world.GetActorByUID(ownerUIDAndName.first);
					Component* foundComponent = owner->FindComponent(ownerUIDAndName.second);

					auto props = foundComponent->GetProps();
					d.Deserialise(props);

					//Decrement the amount of components to spawn
					numObjectsToSpawn--;
				}

				//Deserialise the rest of the components that aren't defined in Actor constructors,
				//but were serialised out through the editor or runtime.
				for (int i = 0; i < numObjectsToSpawn; i++)
				{
					auto component = cs->SpawnComponent(nullptr);

					auto ownerUIDAndName = GetComponentOwnerUIDAndNameOnDeserialise(d);

					component->ownerUID = ownerUIDAndName.first;
					component->name = ownerUIDAndName.second;

					auto props = component->GetProps();
					d.Deserialise(props);

					Actor* owner = world.GetActorByUID(component->ownerUID);
					owner->AddComponent(component);
				}
			}
		}
		else
		{
			IActorSystem* actorSystem = asIt->second;

			std::vector<Actor*> newActors;

			for (int i = 0; i < numObjectsToSpawn; i++)
			{
				Actor* actor = actorSystem->SpawnActor(Transform());
				newActors.push_back(actor);
			}

			//Make sure create()s are after deserialisation
			for (auto actor : newActors)
			{
				auto props = actor->GetProps();
				d.Deserialise(props);

				actor->ResetOwnerUIDToComponents();

				world.AddActorToWorld(actor);
			}
		}
	}

	ResetWorldState();

	debugMenu.AddNotification(VString::wformat(L"%S world loaded", world.worldFilename.c_str()));
}

void FileSystem::ReloadCurrentWorld()
{
	GameInstance::ResetTime();
	GameInstance::DeletePlayerMemories();

	LoadWorld(world.worldFilename);
}

void FileSystem::CreateGameplayWorldSave(std::string worldName)
{
	std::ifstream in("WorldMaps/" + worldName);
	std::ofstream out("GameSaves/" + worldName);

	out << in.rdbuf();
}

void FileSystem::ResetWorldState()
{
	//Deselect any existing actors, because TransformGizmo will stay at previous positions.
	worldEditor.pickedActor = nullptr;
	worldEditor.pickedComponent = nullptr;

	//Clear all commands
	commandSystem.commands.clear();

	//Clear input as well (can carry over to new levels if not cleared)
	Input::Reset();

	battleSystem.Reset();

	//Set player camera on world change as active if in-gameplay
	if (Core::gameplayOn)
	{
		activeCamera = GameUtils::GetPlayer()->camera;
	}

	//Make sure always after camera gets set
	world.Start();

	editor->UpdateWorldList();
	editor->ClearProperties();

	if (Core::gameplayOn) //Don't want actors affected while not in gameplay
	{
		//It's important that this is called after editor::UpdateWorldList() for actor names
		questSystem.ExecuteAllQuestsForCurrentHour();
	}
}
