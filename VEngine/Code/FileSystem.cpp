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
#include "Components/CameraComponent.h"
#include "Actors/Game/Player.h"
#include "VString.h"
#include "Input.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/BattleSystem.h"
#include "Quests/QuestSystem.h"
#include "Profile.h"

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
	auto lastOf = World::worldFilename.find_last_of("/\\");
	std::string str = World::worldFilename.substr(lastOf + 1);

	std::string file = "WorldMaps/" + str;

	if (GameInstance::useGameSaves)
	{
		file = "GameSaves/" + str;
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

	debugMenu.AddNotification(VString::wformat(L"%S world saved", World::worldFilename.c_str()));
}

void FileSystem::WriteAllSystemsToBinary()
{
	auto lastOf = World::worldFilename.find_last_of("/\\");
	std::string str = World::worldFilename.substr(lastOf + 1);

	std::string file = "WorldMaps/Binary/" + str;

	BinarySerialiser s(file);

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
}

//@Todo: binary serialisation isn't working too well. Might be some weird alignment errors taken from property sizes.
void FileSystem::ReadAllSystemsFromBinary()
{
	std::string worldName = World::worldFilename;

	std::string path = "WorldMaps/Binary/" + worldName;

	assert(std::filesystem::exists(path) && "Map file doesn't exist");

	World::Cleanup();

	BinaryDeserialiser d(path);

	while (!d.is.eof())
	{
		std::string systemName;
		d.ReadString(&systemName);

		int numObjectsToSpawn = 0;
		d.Read(&numObjectsToSpawn);

		if (numObjectsToSpawn == 0)
		{
			continue;
		}

		auto actorSystem = actorSystemCache.Get(systemName);
		auto componentSystem = componentSystemCache.Get(systemName);

		if (actorSystem != nullptr)
		{
			for (int i = 0; i < numObjectsToSpawn; i++)
			{
				Actor* actor = actorSystem->SpawnActor(Transform());
			}

			actorSystem->DeserialiseBinary(d);
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
}

void FileSystem::LoadWorld(std::string worldName)
{
	auto startTime = Profile::QuickStart();

	editor->SetEditorTitle(worldName);

	World::worldFilename = worldName;

	std::string path = "WorldMaps/" + worldName;

	if (GameInstance::useGameSaves)
	{
		path = "GameSaves/" + worldName;
	}

	assert(std::filesystem::exists(path) && "Map file doesn't exist");

	World::Cleanup();

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
				for (int i = 0; i < numObjectsToSpawn; i++)
				{
					auto ownerUIDAndName = GetComponentOwnerUIDAndNameOnDeserialise(d);

					Actor* owner = World::GetActorByUID(ownerUIDAndName.first);
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
			IActorSystem* actorSystem = asIt->second;

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
	}

	ResetWorldState();

	debugMenu.AddNotification(VString::wformat(L"%S world loaded", World::worldFilename.c_str()));

	double endTime = Profile::QuickEnd(startTime);
	Log("World load took %f sec.", endTime);
}

void FileSystem::ReloadCurrentWorld()
{
	GameInstance::ResetTime();
	GameInstance::DeletePlayerMemories();

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
	WorldEditor::DeselectAll();

	commandSystem.Reset();
	Input::Reset();
	battleSystem.Reset();

	//Set player camera on world change as active if in-gameplay
	if (Core::gameplayOn)
	{
		activeCamera = GameUtils::GetPlayer()->camera;
	}

	//Make sure always after camera gets set
	World::Start();

	editor->UpdateWorldList();
	editor->ClearProperties();

	if (Core::gameplayOn) //Don't want actors affected while not in gameplay
	{
		//It's important that this is called after editor::UpdateWorldList() for actor names
		questSystem.ExecuteAllQuestsForCurrentHour();
	}
}
