#include "vpch.h"
#include "FileSystem.h"
#include <filesystem>
#include "World.h"
#include "Serialiser.h"
#include "Asset/AssetSystem.h"
#include "Actors/IActorSystem.h"
#include "Actors/ActorSystemCache.h"
#include "WorldEditor.h"
#include "Editor/DebugMenu.h"
#include "Editor/Editor.h"
#include "Commands/CommandSystem.h"
#include "Core.h"
#include "Log.h"
#include "Camera.h"
#include "Actors/Game/Player.h"
#include "Actors/Game/EntranceTrigger.h"
#include "VString.h"
#include "Input.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/WorldFunctions.h"
#include "Profile.h"
#include "UI/UISystem.h"
#include "UI/ScreenFadeWidget.h"
#include <Asset/AssetBaseFolders.h>

static std::string defferedWorldLoadFilename;
static std::string previousWorldMovedFromFilename;
static std::string entranceTriggerTag;

void MovePlayerToEntranceTriggerFromPreviousWorldFilename();

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
	const auto start = Profile::QuickStart();

	auto lastOf = World::worldFilename.find_last_of("/\\");
	std::string str = World::worldFilename.substr(lastOf + 1);

	std::string file = "WorldMaps/" + str;

	if (GameInstance::useGameSaves)
	{
		const std::string previousFilePath = file;
		file = "GameSaves/" + str;
		if (!std::filesystem::exists(file))
		{
			Log("No game save file found for [%s].", file.c_str());
			file = previousFilePath;
		}
	}

	Serialiser s(file, OpenMode::Out);

	for (IActorSystem* actorSystem : World::activeActorSystems)
	{
		if (actorSystem->GetNumActors() > 0)
		{
			actorSystem->Serialise(s);
		}
	}

	for (IComponentSystem* componentSystem : World::activeComponentSystems)
	{
		if (componentSystem->GetNumComponents() > 0)
		{
			componentSystem->Serialise(s);
		}
	}

	s.WriteLine(L"end");

	//AssetSystem::WriteOutAllVertexColourData();

	debugMenu.AddNotification(VString::wformat(L"%S world saved", World::worldFilename.c_str()));

	const auto end = Profile::QuickEnd(start);
	Log("Text save for [%s] took [%f].", str.c_str(), end);
}

void FileSystem::WriteAllSystemsToBinary()
{
	const auto start = Profile::QuickStart();

	auto lastOf = World::worldFilename.find_last_of("/\\");
	std::string str = World::worldFilename.substr(lastOf + 1);

	std::string file = "WorldMaps/Binary/" + str;

	BinarySerialiser s(file.c_str());

	for (IActorSystem* actorSystem : World::activeActorSystems)
	{
		if (actorSystem->GetNumActors() > 0)
		{
			actorSystem->SerialiseBinary(s);
		}
	}

	for (IComponentSystem* componentSystem : World::activeComponentSystems)
	{
		if (componentSystem->GetNumComponents() > 0)
		{
			componentSystem->SerialiseBinary(s);
		}
	}

	debugMenu.AddNotification(VString::wformat(L"%S world saved to binary", World::worldFilename.c_str()));

	const auto end = Profile::QuickEnd(start);
	Log("Binary save for [%s] took [%f].", str.c_str(), end);
}

void FileSystem::ReadAllSystemsFromBinary()
{
	const auto start = Profile::QuickStart();

	std::string worldName = World::worldFilename;

	std::string path = "WorldMaps/Binary/" + worldName;

	assert(std::filesystem::exists(path) && "Map file doesn't exist");

	World::Cleanup();

	BinaryDeserialiser d(path);

	while (!feof(d.file))
	{
		std::string systemName;
		d.ReadString(&systemName);

		size_t numObjectsToSpawn = 0;
		d.Read(&numObjectsToSpawn);

		if (numObjectsToSpawn == 0)
		{
			continue;
		}

		auto actorSystem = ActorSystemCache::Get().GetSystem(systemName);
		auto componentSystem = ComponentSystemCache::Get().GetSystem(systemName);

		if (actorSystem != nullptr)
		{
			for (int i = 0; i < numObjectsToSpawn; i++)
			{
				Actor* actor = actorSystem->SpawnActor(Transform());
				World::RemoveActorFromWorld(actor);

				auto props = actor->GetProps();
				d.Deserialise(props);

				actor->ResetOwnerUIDToComponents();

				World::AddActorToWorld(actor);
			}
		}
		else if (componentSystem != nullptr)
		{
			for (int i = 0; i < numObjectsToSpawn; i++)
			{
				UID ownerUID = 0;
				d.Read(&ownerUID);

				std::string componentName;
				d.ReadString(&componentName);

				Actor* owner = World::GetActorByUID(ownerUID);
				Component* foundComponent = owner->FindComponentAllowNull(componentName);
				if (foundComponent)
				{
					auto props = foundComponent->GetProps();
					d.Deserialise(props);
				}
			}
		}
	}

	ResetWorldState();

	debugMenu.AddNotification(VString::wformat(L"%S world loaded from binary", World::worldFilename.c_str()));

	const auto end = Profile::QuickEnd(start);
	Log("Binary load for [%s] took [%f].", worldName.c_str(), end);
}

void FileSystem::LoadWorld(std::string worldName)
{
	const auto startTime = Profile::QuickStart();

	Editor::Get().SetEditorTitle(worldName);

	GameInstance::previousMapMovedFrom = World::worldFilename;

	World::worldFilename = worldName;

	std::string path = AssetBaseFolders::worldMap + worldName;

	if (GameInstance::useGameSaves)
	{
		path = "GameSaves/" + worldName;
	}

	assert(std::filesystem::exists(path));

	GameUtils::SaveGameInstanceData();

	World::Cleanup();

	Deserialiser d(path, OpenMode::In);

	std::wstring systemName;

	while (d.is >> systemName)
	{
		if (systemName == L"end")
		{
			break;
		}

		size_t numObjectsToSpawn = 0;
		d.is >> numObjectsToSpawn;
		assert(numObjectsToSpawn != 0);

		const std::string stdSystemName = VString::wstos(systemName);

		auto actorSystem = ActorSystemCache::Get().GetSystem(stdSystemName);
		auto componentSystem = ComponentSystemCache::Get().GetSystem(stdSystemName);

		assert(actorSystem != nullptr || componentSystem != nullptr);

		if (actorSystem)
		{
			for (int i = 0; i < numObjectsToSpawn; i++)
			{
				Actor* actor = actorSystem->SpawnActor(Transform());

				//ActorSystem will add in actor. Remove it here before getting correct UID and name on serialise.
				World::RemoveActorFromWorld(actor);

				auto props = actor->GetProps();
				d.Deserialise(props);

				actor->ResetOwnerUIDToComponents();

				World::AddActorToWorld(actor);
			}
		}
		else if (componentSystem)
		{
			//Deserialise the existing components created in Actor constructors
			for (int i = 0; i < numObjectsToSpawn; i++)
			{
				auto ownerUIDAndName = GetComponentOwnerUIDAndNameOnDeserialise(d);

				Actor* owner = World::GetActorByUIDAllowNull(ownerUIDAndName.first);
				if (owner == nullptr)
				{
					Log("Owner %s not found in world for components. Skipping component creations.",
						ownerUIDAndName.second.c_str());

					std::wstring nextToken;
					while (nextToken != L"next" && nextToken != L"end")
					{
						d.is >> nextToken;
					}
				}
				else
				{
					Component* foundComponent = owner->FindComponentAllowNull(ownerUIDAndName.second);
					if (foundComponent)
					{
						auto props = foundComponent->GetProps();
						d.Deserialise(props);
					}
					else //Component doesn't exist on any actor, skip its props
					{
						std::wstring nextToken;
						while (nextToken != L"next" && nextToken != L"end")
						{
							d.is >> nextToken;
						}
					}
				}
			}
		}
		else
		{
			//This will loop over the missing component from an actor.
			//It won't remove the component from the .vmap file though, subsequent 
			//saves will overwrite that missing component instead.

			//Get the previous post so subsequent system name reads work
			std::streampos lastPos = d.is.tellg();

			std::wstring missingProp;

			while (ComponentSystemCache::Get().GetSystem(VString::wstos(missingProp)) ||
				ActorSystemCache::Get().GetSystem(VString::wstos(missingProp)) == nullptr)
			{
				if (missingProp == L"end")
				{
					break;
				}

				lastPos = d.is.tellg();
				d.is >> missingProp;
			}

			d.is.seekg(lastPos);
		}

		systemName.clear();
	}

	ResetWorldState();

	AssetSystem::LoadVertexColourDataFromFile();

	WorldFunctions::CallWorldStartFunction(World::worldFilename);

	Profile::Reset();
	debugMenu.AddNotification(VString::wformat(L"%S world loaded", World::worldFilename.c_str()));

	double endTime = Profile::QuickEnd(startTime);
	Log("World load took %f sec.", endTime);
}

void FileSystem::ReloadCurrentWorld()
{
	LoadWorld(World::worldFilename);
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
	WorldEditor::Reset();

	CommandSystem::Get().Reset();
	Input::Reset();

	//Set player camera on world change as active if in-gameplay
	if (Core::gameplayOn)
	{
		auto player = Player::system.GetFirstActor();
		if (player)
		{
			Camera::SetActiveCamera(player->camera);
		}
	}

	//Make sure always after camera gets set
	World::Start();

	Editor::Get().UpdateWorldList();
	Editor::Get().ClearProperties();
}

void FileSystem::SetDeferredWorldLoad(const std::string_view filename)
{
	defferedWorldLoadFilename = filename;
}

void FileSystem::SetDeferredWorldReset()
{
	defferedWorldLoadFilename = World::worldFilename;
}

void FileSystem::DeferredWorldLoad()
{
	if (!defferedWorldLoadFilename.empty())
	{
		previousWorldMovedFromFilename = World::worldFilename;
		entranceTriggerTag = GameUtils::entranceTriggerTag;

		LoadWorld(defferedWorldLoadFilename);

		MovePlayerToEntranceTriggerFromPreviousWorldFilename();

		defferedWorldLoadFilename.clear();
		previousWorldMovedFromFilename.clear();
		entranceTriggerTag.clear();
		GameUtils::entranceTriggerTag.clear();
	}
}

void MovePlayerToEntranceTriggerFromPreviousWorldFilename()
{
	auto player = Player::system.GetFirstActor();
	if (player == nullptr)
	{
		return;
	}

	std::vector<EntranceTrigger*> possibleEntrancesToMoveTo;

	for (auto& entranceTrigger : EntranceTrigger::system.GetActors())
	{
		if (entranceTrigger->GetLevelToMoveTo() == previousWorldMovedFromFilename)
		{
			possibleEntrancesToMoveTo.emplace_back(entranceTrigger.get());
		}
	}

	const auto MoveToEntrance = [&](EntranceTrigger* entranceTrigger)
		{
			player->SetPosition(entranceTrigger->GetPositionV());
			//Want the opposite rotation on exit for player to face in vs the orientation to enter the entrance
			player->SetRotation(entranceTrigger->GetRotationV());
			player->SetNextPosAndRotToCurrent();
		};

	//Deal with only one potential entrance
	if (possibleEntrancesToMoveTo.size() == 1)
	{
		MoveToEntrance(possibleEntrancesToMoveTo.front());
		UISystem::screenFadeWidget->AddToViewport();
		UISystem::screenFadeWidget->SetToFadeIn();
		return;
	}

	//Deal with multiple entrances. Differentiate by tag.
	for (auto entranceTrigger : possibleEntrancesToMoveTo)
	{
		if (entranceTrigger->GetEntranceTag() == entranceTriggerTag)
		{
			MoveToEntrance(entranceTrigger);
			UISystem::screenFadeWidget->AddToViewport();
			UISystem::screenFadeWidget->SetToFadeIn();
			return;
		}
	}

	Log("No matching EntranceTrigger found on world load.");
}
