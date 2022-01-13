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

FileSystem fileSystem;

void FileSystem::WriteAllActorSystems()
{
	auto lastOf = world.worldFilename.find_last_of("/\\");
	std::string str = world.worldFilename.substr(lastOf + 1);

	std::string file = "WorldMaps/" + str;

	if (GameInstance::useGameSaves)
	{
		file = "GameSaves/" + str;
	}

	Serialiser s(file, OpenMode::Out);

	for (IActorSystem* actorSystem : world.activeActorSystems)
	{
		actorSystem->Serialise(s);
	}

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
			actorSystem->SpawnActor(Transform());
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

		if (actorSystemName.empty())
		{
			break;
		}

		size_t numActorsToSpawn = 0;
		d.is >> numActorsToSpawn;

		auto asIt = actorSystemCache.nameToSystemMap->find(VString::wstos(actorSystemName));
		if (asIt == actorSystemCache.nameToSystemMap->end())
		{
			continue;
		}

		IActorSystem* actorSystem = asIt->second;

		for (int i = 0; i < numActorsToSpawn; i++)
		{
			actorSystem->SpawnActor(Transform());
		}

		actorSystem->Deserialise(d);
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
}
