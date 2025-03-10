export module Core.FileSystem;

import <string>;

export namespace FileSystem
{
	void SerialiseAllSystems();

	void WriteAllSystemsToBinary();
	void ReadAllSystemsFromBinary();

	void LoadWorld(std::string worldName);
	void ReloadCurrentWorld();

	//Creates an equivalent map save to load during gameplay (to avoid having a seperate save file format)
	void CreateGameplayWorldSave(std::string worldName);

	void ResetWorldState();

	void SetDeferredWorldLoad(const std::string_view filename);

	void SetDeferredWorldReset();

	//called at end of the frame to avoid world loads happening inside Tick/Draw functions.
	void DeferredWorldLoad();
};


//Todo: these need to be put into a class FileSystem
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

	auto lastOf = World::Get().GetWorldFilename().find_last_of("/\\");
	std::string str = World::Get().GetWorldFilename().substr(lastOf + 1);

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

	for (IActorSystem* actorSystem : World::Get().GetActiveActorSystems())
	{
		if (actorSystem->GetNumActors() > 0)
		{
			actorSystem->Serialise(s);
		}
	}

	for (IComponentSystem* componentSystem : World::Get().GetActiveComponentSystems())
	{
		if (componentSystem->GetNumComponents() > 0)
		{
			componentSystem->Serialise(s);
		}
	}

	s.WriteLine(L"end");

	//AssetSystem::WriteOutAllVertexColourData();

	debugMenu.AddNotification(VString::wformat(L"%S world saved", World::Get().GetWorldFilename().c_str()));

	const auto end = Profile::QuickEnd(start);
	Log("Text save for [%s] took [%f].", str.c_str(), end);
}

void FileSystem::WriteAllSystemsToBinary()
{
	const auto start = Profile::QuickStart();

	auto lastOf = World::Get().GetWorldFilename().find_last_of("/\\");
	std::string str = World::Get().GetWorldFilename().substr(lastOf + 1);

	std::string file = "WorldMaps/Binary/" + str;

	BinarySerialiser s(file.c_str());

	for (IActorSystem* actorSystem : World::Get().GetActiveActorSystems())
	{
		if (actorSystem->GetNumActors() > 0)
		{
			actorSystem->SerialiseBinary(s);
		}
	}

	for (IComponentSystem* componentSystem : World::Get().GetActiveComponentSystems())
	{
		if (componentSystem->GetNumComponents() > 0)
		{
			componentSystem->SerialiseBinary(s);
		}
	}

	debugMenu.AddNotification(VString::wformat(L"%S world saved to binary", World::Get().GetWorldFilename().c_str()));

	const auto end = Profile::QuickEnd(start);
	Log("Binary save for [%s] took [%f].", str.c_str(), end);
}

void FileSystem::ReadAllSystemsFromBinary()
{
	const auto start = Profile::QuickStart();

	std::string worldName = World::Get().GetWorldFilename();

	std::string path = "WorldMaps/Binary/" + worldName;

	assert(std::filesystem::exists(path) && "Map file doesn't exist");

	World::Get().Cleanup();

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
				World::Get().RemoveActorFromWorld(actor);

				auto props = actor->GetProps();
				d.Deserialise(props);

				actor->ResetOwnerUIDToComponents();

				World::Get().AddActorToWorld(actor);
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

				Actor* owner = World::Get().GetActorByUID(ownerUID);
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

	debugMenu.AddNotification(VString::wformat(L"%S world loaded from binary", World::Get().GetWorldFilename().c_str()));

	const auto end = Profile::QuickEnd(start);
	Log("Binary load for [%s] took [%f].", worldName.c_str(), end);
}

void FileSystem::LoadWorld(std::string worldName)
{
	const auto startTime = Profile::QuickStart();

	Editor::Get().SetEditorTitle(worldName);

	GameInstance::previousMapMovedFrom = World::Get().GetWorldFilename();

	World::Get().SetWorldFilename(worldName);

	std::string path = AssetBaseFolders::worldMap + worldName;

	if (GameInstance::useGameSaves)
	{
		path = "GameSaves/" + worldName;
	}

	assert(std::filesystem::exists(path));

	GameUtils::SaveGameInstanceData();

	World::Get().Cleanup();

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
				World::Get().RemoveActorFromWorld(actor);

				auto props = actor->GetProps();
				d.Deserialise(props);

				actor->ResetOwnerUIDToComponents();

				World::Get().AddActorToWorld(actor);
			}
		}
		else if (componentSystem)
		{
			//Deserialise the existing components created in Actor constructors
			for (int i = 0; i < numObjectsToSpawn; i++)
			{
				auto ownerUIDAndName = GetComponentOwnerUIDAndNameOnDeserialise(d);

				Actor* owner = World::Get().GetActorByUIDAllowNull(ownerUIDAndName.first);
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

	AssetSystem::Get().LoadVertexColourDataFromFile();

	WorldFunctions::CallWorldStartFunction(World::Get().GetWorldFilename());

	Profile::Reset();
	debugMenu.AddNotification(VString::wformat(L"%S world loaded", World::Get().GetWorldFilename().c_str()));

	double endTime = Profile::QuickEnd(startTime);
	Log("World load took %f sec.", endTime);
}

void FileSystem::ReloadCurrentWorld()
{
	LoadWorld(World::Get().GetWorldFilename());
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

	//A world reset should call End() for all actors. Liken it to ending the game.
	World::Get().EndAllActors();

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
	World::Get().Start();

	Editor::Get().UpdateWorldList();
	Editor::Get().ClearProperties();
}

void FileSystem::SetDeferredWorldLoad(const std::string_view filename)
{
	defferedWorldLoadFilename = filename;
}

void FileSystem::SetDeferredWorldReset()
{
	defferedWorldLoadFilename = World::Get().GetWorldFilename();
}

void FileSystem::DeferredWorldLoad()
{
	if (!defferedWorldLoadFilename.empty())
	{
		previousWorldMovedFromFilename = World::Get().GetWorldFilename();
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
