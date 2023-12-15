#include "vpch.h"
#include "WorldEditor.h"
#include <QFileDialog>
#include <QDesktopServices>
#include "Physics/Raycast.h"
#include "Input.h"
#include "Core/VMath.h"
#include "Editor/Editor.h"
#include "Actors/Actor.h"
#include "Actors/IActorSystem.h"
#include "Editor/TransformGizmo.h"
#include "FileSystem.h"
#include "Camera.h"
#include "Actors/MeshActor.h"
#include "Actors/Game/EntranceTrigger.h"
#include "Editor/DebugMenu.h"
#include "Core.h"
#include "Core/Log.h"
#include "Asset/AssetPaths.h"
#include "Render/TextureSystem.h"
#include "Render/Texture2D.h"
#include "Render/Material.h"
#include "Render/MaterialSystem.h"
#include "Components/MeshComponent.h"

std::set<Actor*> pickedActors;
Actor* pickedActor;
SpatialComponent* pickedComponent;
IActorSystem* spawnSystem;
std::string actorTemplateFilename;
WorldEditor::PickMode pickMode = WorldEditor::PickMode::Actor;

bool WorldEditor::texturePlacement = false;
bool WorldEditor::meshPlacement = false;
bool WorldEditor::materialPlacement = false;
bool WorldEditor::vertexPaintActive = false;
bool WorldEditor::uvPaintActive = false;
bool WorldEditor::uvPaintOn = false;
bool WorldEditor::actorReplaceModeActive = false;
bool WorldEditor::parentSetActive = false;
bool WorldEditor::moveActorViaKeyboardInput = false;

UVPaintData WorldEditor::uvPaintData;

//Load world based on EntranceTrigger's props on actor click in-world.
bool WorldEditor::entranceTriggerWorldLoadMode = false;

DirectX::XMFLOAT4 WorldEditor::vertexPaintColour = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
bool WorldEditor::vertexPaintFaceFillMode = false;
Actor* WorldEditor::vertexPaintLockActor;

void HandleActorPicking();
void DuplicateActor();
void SaveWorld();
void DeleteActor();
void SpawnActorOnClick();
void SpawnActor(Transform& transform);
void VertexPainting();
void UVPainting();
void SetParentOnClick(Actor& hitActor);
void MoveActorViaKeyboardInput();
void LoadWorldOnEntranceTriggerClick(Actor* pickedActor);
void QuickMeshChangeMenu();
void QuickTextureChangeMenu();

void WorldEditor::Tick()
{
	if (!debugMenu.hasMouseFocus)
	{
		SpawnActorOnClick();
		HandleActorPicking();
		VertexPainting();
		UVPainting();
	}

	DuplicateActor();
	DeleteActor();

	MoveActorViaKeyboardInput();

	QuickMeshChangeMenu();
	QuickTextureChangeMenu();

	SaveWorld();
}

void WorldEditor::Reset()
{
	DeselectAll();

	pickedActors.clear();
	pickedComponent = nullptr;
	actorTemplateFilename.clear();
}

void HandleActorPicking()
{
	if (transformGizmo.CheckMouseOver() && transformGizmo.CheckInUse()
		|| Core::gameplayOn || WorldEditor::vertexPaintActive || WorldEditor::uvPaintActive)
	{
		return;
	}

	if (Input::GetMouseLeftUp())
	{
		HitResult hit;
		if (RaycastFromScreen(hit))
		{
			//@Todo: move all these 'placement' blocks into functions

			SetParentOnClick(*hit.hitActor);

			//Assign selected texture in editor to mesh on click
			if (!TextureSystem::selectedTextureInEditor.empty() && WorldEditor::texturePlacement)
			{
				auto mesh = dynamic_cast<MeshComponent*>(hit.hitComponent);
				if (mesh)
				{
					mesh->GetMaterial().materialShaderData.useTexture = true;
					mesh->SetTexture(VString::wstos(TextureSystem::selectedTextureInEditor));
					return;
				}
			}

			if (!MeshActor::spawnMeshFilename.empty() && WorldEditor::meshPlacement)
			{
				auto mesh = dynamic_cast<MeshComponent*>(hit.hitComponent);
				if (mesh)
				{
					mesh->meshComponentData.filename = MeshActor::spawnMeshFilename;
					//@Todo: Even through this code block won't be hit during game runtime, it's still a leak
					//to only recreate the GPU buffers without releasing them. Either make a MeshComponent::Cleanup()
					//or figure something else out.
					mesh->Create();
					return;
				}
			}

			//Assign selected material in editor to mesh on click
			if (!MaterialSystem::selectedMaterialInEditor.empty() && WorldEditor::materialPlacement)
			{
				auto mesh = dynamic_cast<MeshComponent*>(hit.hitComponent);
				if (mesh)
				{
					Material loadedMaterial = MaterialSystem::LoadMaterialFromFile(MaterialSystem::selectedMaterialInEditor);
					loadedMaterial.Create();
					mesh->GetMaterial() = loadedMaterial;
					return;
				}
			}

			switch (pickMode)
			{
			case WorldEditor::PickMode::Actor:
			{
				if (Input::GetKeyHeld(Keys::Ctrl) || pickedActors.empty())
				{
					pickedActors.insert(hit.hitActor);
				}
				else
				{
					pickedActors.clear();
				}

				WorldEditor::SetPickedActor(hit.hitActor);
				break;
			}
			case WorldEditor::PickMode::Component:
			{
				WorldEditor::SetPickedComponent(hit.hitComponent);
				break;
			}
			}

			//Handle actor replacement 
			if (WorldEditor::actorReplaceModeActive)
			{
				if (spawnSystem != nullptr)
				{
					auto pickedActor = WorldEditor::GetPickedActor();
					if (pickedActor)
					{
						auto pickedActorTransform = pickedActor->GetTransform();
						pickedActor->Remove();

						auto replacementActor = spawnSystem->SpawnActor(pickedActorTransform);
						replacementActor->Create();
						replacementActor->CreateAllComponents();
						replacementActor->PostCreate();

						editor->UpdateWorldList();

						WorldEditor::ClearPickedActors();
						WorldEditor::SetPickedActor(replacementActor);
					}
				}
				else
				{
					Log("Spawn System not set for actor replacement.");
				}
			}

			//Make sure this call is last. Has the potential to load new map.
			LoadWorldOnEntranceTriggerClick(hit.hitActor);
		}
	}
}

void DuplicateActor()
{
	if (Input::GetKeyHeld(Keys::Ctrl))
	{
		if (Input::GetKeyDown(Keys::W))
		{
			if (pickedActor)
			{
				pickedActors.clear();

				editor->ClearProperties();

				Transform transform = pickedActor->GetTransform();
				Actor* newDuplicateActor = pickedActor->GetActorSystem()->SpawnActor(transform);

				//Copy values across
				auto oldProps = pickedActor->GetAllProps();
				auto newProps = newDuplicateActor->GetAllProps();
				Properties::CopyProperties(oldProps, newProps);

				newDuplicateActor->Create();
				newDuplicateActor->CreateAllComponents();
				newDuplicateActor->PostCreate();

				//@Todo: move to Paint debug menu
				auto newDuplicateActorMeshes = newDuplicateActor->GetComponents<MeshComponent>();
				for (auto mesh : newDuplicateActorMeshes)
				{
					auto matchingMesh = pickedActor->GetComponent<MeshComponent>(mesh->name);
					if (matchingMesh)
					{
						mesh->meshDataProxy.vertices = matchingMesh->meshDataProxy.vertices;
					}
					mesh->CreateNewVertexBuffer();
				}

				World::AddActorToWorld(newDuplicateActor);

				editor->SetActorProps(newDuplicateActor);
				editor->UpdateWorldList();

				debugMenu.AddNotification(VString::wformat(
					L"Duplicated new actor [%S]", newDuplicateActor->GetName().c_str()));

				//Set new actor as picked in-editor
				pickedActor = newDuplicateActor;
			}
		}
	}
}

void SaveWorld()
{
	if (!Core::gameplayOn)
	{
		if (Input::GetKeyHeld(Keys::Ctrl))
		{
			if (Input::GetKeyDown(Keys::S))
			{
				FileSystem::SerialiseAllSystems();
			}
		}
	}
}

void DeleteActor()
{
	if (Input::GetKeyUp(Keys::Delete) ||
		(Input::GetKeyHeld(Keys::Ctrl) && Input::GetKeyDown(Keys::D)))
	{
		switch (pickMode)
		{
		case WorldEditor::PickMode::Actor:
		{
			if (pickedActor)
			{
				editor->RemoveActorFromWorldList();

				if (pickedActors.size() > 1)
				{
					//Destroy all multiple picked actors
					for (auto actor : pickedActors)
					{
						actor->Remove();
					}
				}
				else
				{
					debugMenu.AddNotification(VString::wformat(
						L"Destroyed actor [%S]", pickedActor->GetName().c_str()));
					pickedActor->Remove();
				}
			}

			break;
		}
		case WorldEditor::PickMode::Component:
		{
			if (pickedComponent)
			{
				pickedComponent->Remove();
			}

			break;
		}
		}

		pickedActors.clear();
		pickedActor = nullptr;
		pickedComponent = nullptr;

		editor->ClearProperties();
	}
}

//Spawn actor on middle mouse click in viewport
void SpawnActorOnClick()
{
	if (Input::GetMouseMiddleUp())
	{
		if (spawnSystem)
		{
			HitResult hit;
			if (RaycastFromScreen(hit))
			{
				Transform transform;
				XMStoreFloat3(&transform.position, hit.GetHitPosV());
				VMath::CeilFloat3(transform.position);
				SpawnActor(transform);
			}
			else //Spawn actor a bit in front of the camera based on the click
			{
				auto& activeCamera = Camera::GetActiveCamera();

				XMVECTOR spawnPos = XMLoadFloat3(&activeCamera.transform.position);
				const XMFLOAT3 forward = activeCamera.GetForwardVector();
				const XMVECTOR forwardVec = XMLoadFloat3(&forward);
				spawnPos += forwardVec * 10.0f;

				const XMVECTOR dist = hit.direction * 10.f;
				const XMVECTOR rayEnd = hit.origin + dist;

				Transform transform;
				XMStoreFloat3(&transform.position, rayEnd);
				VMath::RoundFloat3(transform.position);

				SpawnActor(transform);
			}

			editor->UpdateWorldList();
			pickedActors.clear();
		}
	}
}

void SpawnActor(Transform& transform)
{
	Actor* actor = nullptr;

	if (!actorTemplateFilename.empty()) //Spawn actor through template
	{
		actor = WorldEditor::SpawnActorFromTemplateFile(actorTemplateFilename, transform);
	}
	else //Spawn Actor from System
	{
		actor = spawnSystem->SpawnActor(transform);
		debugMenu.AddNotification(VString::wformat(
			L"Spawned actor [%S] from [%S] system", actor->GetName().c_str(), spawnSystem->GetName().c_str()));
	}

	actor->Create();
	actor->CreateAllComponents();
	actor->PostCreate();

	pickedActor = actor;
	editor->SetActorProps(pickedActor);
}

void VertexPainting()
{
	if (WorldEditor::vertexPaintActive)
	{
		if (Input::GetMouseLeftDown() && Input::GetKeyHeld(Keys::Ctrl)) //Set colour of hit vertex to paint colour
		{
			HitResult hit;
			if (RaycastFromScreen(hit))
			{
				if (WorldEditor::vertexPaintLockActor != nullptr)
				{
					if (hit.hitActor != WorldEditor::vertexPaintLockActor)
					{
						return;
					}
				}

				auto meshes = hit.hitActor->GetComponents<MeshComponent>();
				for (auto mesh : meshes)
				{
					for (auto& vertIndex : hit.hitVertIndexes)
					{
						Vertex v = mesh->meshDataProxy.vertices.at(vertIndex);
						WorldEditor::vertexPaintColour = v.colour;
					}
				}
			}
		}
		else if (Input::GetMouseLeftDown()) //Paint
		{
			HitResult hit;
			if (RaycastFromScreen(hit))
			{
				if (WorldEditor::vertexPaintLockActor != nullptr)
				{
					if (hit.hitActor != WorldEditor::vertexPaintLockActor)
					{
						return;
					}
				}

				auto meshes = hit.hitActor->GetComponents<MeshComponent>();
				for (auto mesh : meshes)
				{
					const auto numVerts = mesh->meshDataProxy.vertices.size();
					auto& vertices = mesh->meshDataProxy.GetVertices();

					//Set indicies based on face fill or single vertex
					std::vector<int> vertIndexes;
					if (WorldEditor::vertexPaintFaceFillMode)
					{
						vertIndexes = hit.vertIndexesOfHitTriangleFace;
					}
					else
					{
						vertIndexes = hit.hitVertIndexes;
					}

					for (const int vertIndex : vertIndexes)
					{
						if (vertIndex >= numVerts)
						{
							continue;
						}
						Vertex& v = vertices.at(vertIndex);
						v.colour = WorldEditor::vertexPaintColour;
					}

					mesh->CreateNewVertexBuffer();
				}
			}
		}
	}
}

void UVPainting()
{
	if (!WorldEditor::uvPaintActive) return;

	if (!WorldEditor::uvPaintOn) return;

	const bool leftClick = Input::GetMouseLeftDown();
	const bool rightClick = Input::GetMouseRightDown();

	if (leftClick || rightClick)
	{
		HitResult hit;
		if (RaycastFromScreen(hit))
		{
			if (hit.hitActor != WorldEditor::GetPickedActor()) return;

			auto meshes = hit.hitActor->GetComponents<MeshComponent>();
			for (auto mesh : meshes)
			{
				const auto numVerts = mesh->meshDataProxy.vertices.size();
				auto& vertices = mesh->meshDataProxy.GetVertices();

				assert(hit.vertIndexesOfHitTriangleFace.size() == 3);
				std::vector<XMFLOAT2*> newUVs;

				std::vector<Vertex> newVerts;

				for (int vertIndex : hit.vertIndexesOfHitTriangleFace)
				{
					auto& vertex = vertices.at(vertIndex);
					newVerts.push_back(vertex);
					newUVs.push_back(&vertex.uv);
				}

				auto texture = TextureSystem::FindTexture2D(WorldEditor::uvPaintData.texture);
				const auto textureWidth = texture->GetWidth();
				const auto textureHeight = texture->GetHeight();

				assert(newUVs.size() == 3);

				//The uv values here are very finiky. Keep an eye on them.
				if (leftClick)
				{
					switch (WorldEditor::uvPaintData.uvPaintRotate)
					{
					case UVPaintRotate::Left:
					{
						const float u0 = WorldEditor::uvPaintData.x / textureWidth;
						const float v0 = WorldEditor::uvPaintData.y / textureHeight;
						const float u1 = WorldEditor::uvPaintData.w / textureWidth;
						const float v1 = WorldEditor::uvPaintData.y / textureHeight;
						const float u2 = WorldEditor::uvPaintData.w / textureWidth;
						const float v2 = WorldEditor::uvPaintData.h / textureHeight;
						*newUVs[0] = XMFLOAT2(u0, v0);
						*newUVs[2] = XMFLOAT2(u1, v1);
						*newUVs[1] = XMFLOAT2(u2, v2);
						break;
					}
					case UVPaintRotate::Right:
					{
						const float u0 = WorldEditor::uvPaintData.x / textureWidth;
						const float v0 = WorldEditor::uvPaintData.y / textureHeight;
						const float u1 = WorldEditor::uvPaintData.w / textureWidth;
						const float v1 = WorldEditor::uvPaintData.h / textureHeight;
						const float u2 = WorldEditor::uvPaintData.x / textureWidth;
						const float v2 = WorldEditor::uvPaintData.h / textureHeight;
						*newUVs[1] = XMFLOAT2(u0, v0);
						*newUVs[0] = XMFLOAT2(u1, v1);
						*newUVs[2] = XMFLOAT2(u2, v2);
						break;
					}
					case UVPaintRotate::Up:
					{
						const float u0 = WorldEditor::uvPaintData.w / textureWidth;
						const float v0 = WorldEditor::uvPaintData.h / textureHeight;
						const float u1 = WorldEditor::uvPaintData.x / textureWidth;
						const float v1 = WorldEditor::uvPaintData.y / textureHeight;
						const float u2 = WorldEditor::uvPaintData.w / textureWidth;
						const float v2 = WorldEditor::uvPaintData.y / textureHeight;
						*newUVs[0] = XMFLOAT2(u0, v0);
						*newUVs[1] = XMFLOAT2(u1, v2);
						*newUVs[2] = XMFLOAT2(u2, v2);
						break;
					}
					case UVPaintRotate::Down:
					{
						const float u0 = WorldEditor::uvPaintData.x / textureWidth;
						const float v0 = WorldEditor::uvPaintData.y / textureHeight;
						const float u1 = WorldEditor::uvPaintData.w / textureWidth;
						const float v1 = WorldEditor::uvPaintData.h / textureHeight;
						const float u2 = WorldEditor::uvPaintData.x / textureWidth;
						const float v2 = WorldEditor::uvPaintData.h / textureHeight;
						*newUVs[0] = XMFLOAT2(u0, v0);
						*newUVs[1] = XMFLOAT2(u1, v1);
						*newUVs[2] = XMFLOAT2(u2, v2);
						break;
					}
					}
				}
				else if (rightClick)
				{
					switch (WorldEditor::uvPaintData.uvPaintRotate)
					{
					case UVPaintRotate::Left:
					{
						const float u0 = WorldEditor::uvPaintData.x / textureWidth;
						const float v0 = WorldEditor::uvPaintData.y / textureHeight;
						const float u1 = WorldEditor::uvPaintData.w / textureWidth;
						const float v1 = WorldEditor::uvPaintData.h / textureHeight;
						const float u2 = WorldEditor::uvPaintData.x / textureWidth;
						const float v2 = WorldEditor::uvPaintData.h / textureHeight;
						*newUVs[0] = XMFLOAT2(u0, v0);
						*newUVs[2] = XMFLOAT2(u1, v1);
						*newUVs[1] = XMFLOAT2(u2, v2);
						break;
					}
					case UVPaintRotate::Right:
					{
						const float u0 = WorldEditor::uvPaintData.x / textureWidth;
						const float v0 = WorldEditor::uvPaintData.y / textureHeight;
						const float u1 = WorldEditor::uvPaintData.w / textureWidth;
						const float v1 = WorldEditor::uvPaintData.y / textureHeight;
						const float u2 = WorldEditor::uvPaintData.w / textureWidth;
						const float v2 = WorldEditor::uvPaintData.h / textureHeight;
						*newUVs[2] = XMFLOAT2(u0, v0);
						*newUVs[1] = XMFLOAT2(u1, v1);
						*newUVs[0] = XMFLOAT2(u2, v2);
						break;
					}
					case UVPaintRotate::Up:
					{
						const float u0 = WorldEditor::uvPaintData.x / textureWidth;
						const float v0 = WorldEditor::uvPaintData.y / textureHeight;
						const float u1 = WorldEditor::uvPaintData.w / textureWidth;
						const float v1 = WorldEditor::uvPaintData.h / textureHeight;
						const float u2 = WorldEditor::uvPaintData.x / textureWidth;
						const float v2 = WorldEditor::uvPaintData.h / textureHeight;
						*newUVs[2] = XMFLOAT2(u0, v0);
						*newUVs[0] = XMFLOAT2(u1, v1);
						*newUVs[1] = XMFLOAT2(u2, v2);
						break;
					}
					case UVPaintRotate::Down:
					{
						const float u0 = WorldEditor::uvPaintData.x / textureWidth;
						const float v0 = WorldEditor::uvPaintData.y / textureHeight;
						const float u1 = WorldEditor::uvPaintData.w / textureWidth;
						const float v1 = WorldEditor::uvPaintData.y / textureHeight;
						const float u2 = WorldEditor::uvPaintData.w / textureWidth;
						const float v2 = WorldEditor::uvPaintData.h / textureHeight;
						*newUVs[0] = XMFLOAT2(u0, v0);
						*newUVs[1] = XMFLOAT2(u1, v1);
						*newUVs[2] = XMFLOAT2(u2, v2);
						break;
					}
					}
				}

				mesh->SetTexture(WorldEditor::uvPaintData.texture);

				mesh->CreateNewVertexBuffer();
			}
		}
	}
}

void SetParentOnClick(Actor& hitActor)
{
	if (WorldEditor::parentSetActive && pickedActor != nullptr)
	{
		hitActor.AddChild(pickedActor);
		Log("[%s] added as child to [%s].", pickedActor->GetName().c_str(), hitActor.GetName().c_str());
	}
}

void MoveActorViaKeyboardInput()
{
	if (pickedActor == nullptr || !WorldEditor::moveActorViaKeyboardInput) {
		return;
	}

	if (Input::GetKeyDown(Keys::W)) {
		pickedActor->AddPositionV(VMath::GlobalForwardVector());
	}
	else if (Input::GetKeyDown(Keys::S)) {
		pickedActor->AddPositionV(-VMath::GlobalForwardVector());
	}
	else if (Input::GetKeyDown(Keys::A)) {
		pickedActor->AddPositionV(-VMath::GlobalRightVector());
	}
	else if (Input::GetKeyDown(Keys::D)) {
		pickedActor->AddPositionV(VMath::GlobalRightVector());
	}

	if (Input::GetKeyDown(Keys::Down)) {
		pickedActor->AddRotation(VMath::GlobalRightVector(), -90.f);
	}
	else if (Input::GetKeyDown(Keys::Up)) {
		pickedActor->AddRotation(VMath::GlobalRightVector(), 90.f);
	}
	else if (Input::GetKeyDown(Keys::Left)) {
		pickedActor->AddRotation(VMath::GlobalUpVector(), -90.f);
	}
	else if (Input::GetKeyDown(Keys::Right)) {
		pickedActor->AddRotation(VMath::GlobalUpVector(), 90.f);
	}
}

void LoadWorldOnEntranceTriggerClick(Actor* pickedActor)
{
	if (WorldEditor::entranceTriggerWorldLoadMode)
	{
		auto entranceTrigger = dynamic_cast<EntranceTrigger*>(pickedActor);
		if (entranceTrigger)
		{
			FileSystem::LoadWorld(entranceTrigger->GetLevelToMoveTo());
		}
	}
}

void QuickMeshChangeMenu()
{
	if (Input::GetKeyHeld(Keys::Ctrl) && Input::GetKeyDown(Keys::M))
	{
		auto pickedActor = WorldEditor::GetPickedActor();
		if (pickedActor != nullptr)
		{
			QFileDialog dialog;
			dialog.setFileMode(QFileDialog::AnyFile);

			const QString filePath = dialog.getOpenFileName(nullptr, "Set Mesh",
				QString::fromStdString(AssetBaseFolders::mesh), nullptr, nullptr,
				QFileDialog::Option::DontUseNativeDialog);
			if (!filePath.isEmpty())
			{
				const std::string meshFilename =
					VString::GetSubStringAtFoundOffset(filePath.toStdString(), AssetBaseFolders::mesh);

				for (auto mesh : pickedActor->GetComponents<MeshComponent>())
				{
					mesh->SetMeshFilename(meshFilename);
					mesh->ReCreate();
					Log("[%s] mesh [%u] changed mesh filename to [%s].",
						pickedActor->GetName().c_str(), mesh->GetUID(), meshFilename.c_str());
				}
			}
		}

		Input::ResetHeldKeys();
	}
}

void QuickTextureChangeMenu()
{
	if (Input::GetKeyHeld(Keys::Ctrl) && Input::GetKeyDown(Keys::T))
	{
		auto pickedActor = WorldEditor::GetPickedActor();
		if (pickedActor != nullptr)
		{
			QFileDialog dialog;
			dialog.setFileMode(QFileDialog::AnyFile);

			const QString filePath = dialog.getOpenFileName(nullptr, "Set Texture",
				QString::fromStdString(AssetBaseFolders::texture), nullptr, nullptr);
			if (!filePath.isEmpty())
			{
				const std::string textureFilename =
					VString::GetSubStringAtFoundOffset(filePath.toStdString(), AssetBaseFolders::texture);

				for (auto mesh : pickedActor->GetComponents<MeshComponent>())
				{
					mesh->SetTexture(textureFilename);
					Log("[%s] mesh [%u] changed mesh texture to [%s].",
						pickedActor->GetName().c_str(), mesh->GetUID(), textureFilename.c_str());
				}
			}
		}

		Input::ResetHeldKeys();
	}
}

void WorldEditor::DeselectPickedActor()
{
	pickedActor = nullptr;
	editor->ClearProperties();
}

void WorldEditor::DeselectAll()
{
	DeselectPickedActor();
	pickedComponent = nullptr;
}

void WorldEditor::SetPickedActor(Actor* actor)
{
	assert(actor);
	pickedActor = actor;

	pickedActors.insert(actor);

	editor->SetActorProps(pickedActor);
	editor->SelectActorInWorldList();
}

void WorldEditor::SetPickedComponent(SpatialComponent* spatialComponent)
{
	assert(spatialComponent);
	pickedComponent = spatialComponent;
}

Actor* WorldEditor::GetPickedActor()
{
	return pickedActor;
}

std::set<Actor*>& WorldEditor::GetPickedActors()
{
	return pickedActors;
}

WorldEditor::PickMode WorldEditor::GetPickMode()
{
	return pickMode;
}

SpatialComponent* WorldEditor::GetPickedComponent()
{
	return pickedComponent;
}

void WorldEditor::SetPickMode(PickMode newPickMode)
{
	pickMode = newPickMode;
}

void WorldEditor::SetSpawnSystem(IActorSystem* newSpawnSystem)
{
	assert(newSpawnSystem);
	spawnSystem = newSpawnSystem;
}

IActorSystem* WorldEditor::GetSpawnSystem()
{
	return spawnSystem;
}

void WorldEditor::AddPickedActor(Actor* actor)
{
	assert(actor);
	pickedActors.insert(actor);
}

void WorldEditor::ClearPickedActors()
{
	pickedActors.clear();
}

std::string WorldEditor::GetActorTempateFilename()
{
	return actorTemplateFilename;
}

void WorldEditor::SetActorTemplateFilename(std::string netActorTempateFilename)
{
	actorTemplateFilename = netActorTempateFilename;
}

Actor* WorldEditor::SpawnActorFromTemplateFile(std::string templateFilename, Transform& transform)
{
	std::string path = AssetBaseFolders::actorTemplate + templateFilename;
	Deserialiser d(path, OpenMode::In);

	std::wstring actorSystemName;
	d.is >> actorSystemName;

	IActorSystem* actorSystem = ActorSystemCache::Get().GetSystem(VString::wstos(actorSystemName));

	Actor* actor = actorSystem->SpawnActor(transform);

	auto actorProps = actor->GetProps();

	d.Deserialise(actorProps);

	actor->SetUID(GenerateUID());
	actor->ResetOwnerUIDToComponents();

	auto actorComponents = actor->GetAllComponents();
	for (auto component : actorComponents)
	{
		auto componentProps = component->GetProps();
		d.Deserialise(componentProps);
		component->Create();
	}

	//Set the transform, props will have the original transform data.
	actor->SetTransform(transform);

	std::string newActorName = actorSystem->GetName() + std::to_string(actorSystem->GetNumActors() - 1);
	actor->SimpleSetName(newActorName);

	const std::string spawnMessage =
		VString::format("Spawned actor [%s] from template", actor->GetName().c_str());
	debugMenu.AddNotification(VString::stows(spawnMessage));

	return actor;
}

