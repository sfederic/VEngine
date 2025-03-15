import Core.World;
import Actors.ActorSystemCache;
import Core.FileSystem;
import Core.UID;
import Core.Core;
import Render.TextureSystem;
import Render.DebugMeshManager;

void World::Init()
{
	for (auto actorSystem : ActorSystemCache::Get().GetAllSystems())
	{
		activeActorSystems.emplace_back(actorSystem);
	}

	for (auto componentSystem : ComponentSystemCache::Get().GetAllSystems())
	{
		activeComponentSystems.emplace_back(componentSystem);
	}

	//Load starting map
	FileSystem::LoadWorld(GameInstance::startingMap);
}

void World::Start()
{
	ResetUIDCache();

	TextureSystem::CreateAllTextures();
	DebugMeshManager::CreateDebugMeshes();

	for (auto actorSystem : activeActorSystems)
	{
		actorSystem->Init();
	}

	for (auto componentSystem : activeComponentSystems)
	{
		componentSystem->Init();
	}

	for (auto actorSystem : activeActorSystems)
	{
		actorSystem->PostInit();
	}

	if (Core::gameplayOn)
	{
		StartAllComponents();
		WakeAndStartAllActors();

		GameUtils::LoadGameInstanceData();

		UISystem::mapInfoWidget->AddToViewport(3.f);
	}
}

void World::WakeAndStartAllActors()
{
	auto actors = GetAllActorsInWorld();

	for (auto actor : actors)
	{
		actor->Awake();
	}

	for (auto actor : actors)
	{
		actor->Start();
	}

	for (auto actor : actors)
	{
		actor->LateStart();
	}
}

void World::StartAllComponents()
{
	auto components = GetAllComponentsInWorld();

	for (auto component : components)
	{
		component->Start();
	}
}

void World::EndAllActors()
{
	for (auto actor : GetAllActorsInWorld())
	{
		actor->End();
	}
}

void World::DestroyAllDeferredActors()
{
	for (auto actorSystem : activeActorSystems)
	{
		actorSystem->DestroyDeferredActors();
	}
}

//Note that this function is game specific.
void World::CreateDefaultMapActors()
{
	auto player = Player::system.Add();
	player->Create();
	player->CreateAllComponents();

	auto dlight = DirectionalLightActor::system.Add();
	dlight->Create();
	dlight->CreateAllComponents();
	//Set light pointing down because shadows looks nice.
	dlight->SetRotation(XMVectorSet(0.354f, 0.354f, -0.14f, 0.853f));
	dlight->SetPosition(XMVectorSet(0.f, 5.f, 0.f, 1.f));

	auto grid = Grid::system.Add();
	grid->SetGridSize(5, 5);
	grid->Create();
	grid->CreateAllComponents();

	//Set a ground plane to work off
	MeshActor::spawnMeshFilename = "node.vmesh";
	auto mesh = MeshActor::system.Add();
	mesh->Create();
	mesh->CreateAllComponents();
	mesh->SetPosition(XMFLOAT3(2.f, -0.5f, 2.f));
	mesh->SetScale(XMVectorSet(5.f, 1.f, 5.f, 1.0f));

	MeshActor::spawnMeshFilename.clear();

	Editor::Get().UpdateWorldList();
}

std::vector<IActorSystem*> World::GetLayerActorSystems()
{
	std::vector<IActorSystem*> actorSystems;

	for (auto actorSystem : activeActorSystems)
	{
		if (actorSystem->GetName() == "EntranceTrigger" ||
			actorSystem->GetName() == "MeshActor")
		{
			continue;
		}

		actorSystems.emplace_back(actorSystem);
	}

	return actorSystems;
}

void World::TickAllActorSystems(float deltaTime)
{
	Profile::Start();
	for (auto actorSystem : activeActorSystems)
	{
		actorSystem->Tick(deltaTime);
	}
	Profile::End();
}

void World::TickAllComponentSystems(float deltaTime)
{
	Profile::Start();
	for (auto componentSystem : activeComponentSystems)
	{
		componentSystem->Tick(deltaTime);
	}
	Profile::End();
}

std::vector<Actor*> World::GetAllActorsInWorld()
{
	std::vector<Actor*> outActors;
	for (auto actorSystem : activeActorSystems)
	{
		if (actorSystem->GetNumActors() == 0) continue;
		auto actors = actorSystem->GetActorsAsBaseClass();
		outActors.insert(outActors.end(), actors.begin(), actors.end());
	}
	return outActors;
}

Actor* World::GetActorByUID(UID uid)
{
	auto actorIt = actorUIDMap.find(uid);
	assert(actorIt != actorUIDMap.end());
	return actorIt->second;
}

Actor* World::GetActorByUIDAllowNull(UID uid)
{
	auto actorIt = actorUIDMap.find(uid);
	if (actorIt == actorUIDMap.end())
	{
		return nullptr;
	}
	return actorIt->second;
}

Actor* World::GetActorByName(std::string actorName)
{
	auto actorIt = actorNameMap.find(actorName);
	assert(actorIt != actorNameMap.end());
	return actorIt->second;
}

Actor* World::GetActorByNameAllowNull(std::string actorName)
{
	auto foundActorIt = actorNameMap.find(actorName);
	if (foundActorIt == actorNameMap.end())
	{
		Log("%s actor not found.", actorName.c_str());
		return nullptr;
	}
	return foundActorIt->second;
}

std::vector<Component*> World::GetAllComponentsInWorld()
{
	std::vector<Component*> outComponents;
	for (IComponentSystem* componentSystem : activeComponentSystems)
	{
		if (componentSystem->GetNumComponents() == 0) continue;
		auto components = componentSystem->GetComponentsAsBaseClass();
		outComponents.insert(outComponents.end(), components.begin(), components.end());
	}
	return outComponents;
}

void World::AddActorToWorld(Actor* actor)
{
	std::string actorName = actor->GetName();
	UID actorUID = actor->GetUID();

	actorUIDMap.emplace(actorUID, actor);
	actorNameMap.emplace(actorName, actor);
}

void World::RemoveActorFromWorld(Actor* actor)
{
	actorUIDMap.erase(actor->GetUID());
	actorNameMap.erase(actor->GetName());
}

void World::RemoveActorFromWorld(UID actorUID)
{
	Actor* actor = actorUIDMap.find(actorUID)->second;
	actorNameMap.erase(actor->GetName());
	actorUIDMap.erase(actorUID);
}

void World::RemoveActorFromWorld(std::string actorName)
{
	Actor* actor = actorNameMap.find(actorName)->second;
	actorUIDMap.erase(actor->GetUID());
	actorNameMap.erase(actorName);
}

void World::ClearAllActorsFromWorld()
{
	actorUIDMap.clear();
	actorNameMap.clear();
}

void World::DestroyAllActorsAndComponentsInWorld()
{
	for (auto componentSystem : activeComponentSystems)
	{
		componentSystem->DestroyAll();
		componentSystem->Cleanup();
	}

	for (auto actorSystem : activeActorSystems)
	{
		actorSystem->DestroyAll();
		actorSystem->Cleanup();
	}

	ClearAllActorsFromWorld();

	Editor::Get().UpdateWorldList();
}

bool World::CheckIfActorExistsInWorld(std::string actorName)
{
	return actorNameMap.find(actorName) != actorNameMap.end();
}

bool World::CheckIfActorExistsInWorld(UID actorUID)
{
	return actorUIDMap.find(actorUID) != actorUIDMap.end();
}

void World::Cleanup()
{
	actorUIDMap.clear();
	actorNameMap.clear();

	for (auto componentSystem : activeComponentSystems)
	{
		componentSystem->DestroyAll();
		componentSystem->Cleanup();
	}

	for (auto actorSystem : activeActorSystems)
	{
		actorSystem->DestroyAll();
		actorSystem->Cleanup();
	}

	DebugMeshManager::DestroyDebugMeshes();

	Timer::Cleanup();
	PhysicsSystem::Reset();
	AudioSystem::DeleteLoadedAudioAndChannels();
	TextureSystem::Cleanup();
	MaterialSystem::Cleanup();
	SpriteSystem::Reset();
	UISystem::Reset();
	AssetSystem::Get().ResetMeshData();
}
