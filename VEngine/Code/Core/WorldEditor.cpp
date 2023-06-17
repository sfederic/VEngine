#include "vpch.h"
#include "WorldEditor.h"
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
#include "Editor/DebugMenu.h"
#include "Core.h"
#include "Asset/AssetPaths.h"
#include "Render/TextureSystem.h"
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
bool WorldEditor::actorReplaceModeActive = false;

DirectX::XMFLOAT4 WorldEditor::vertexPaintColour;

void HandleActorPicking();
void DuplicateActor();
void SaveWorld();
void DeleteActor();
void SpawnActorOnClick();
void SpawnActor(Transform& transform);
void VertexPainting();

void WorldEditor::Tick()
{
	if (!debugMenu.hasMouseFocus)
	{
		SpawnActorOnClick();
		HandleActorPicking();
		VertexPainting();
	}

	DuplicateActor();
	DeleteActor();

	SaveWorld();
}

void WorldEditor::Reset()
{
	DeselectAll();

	pickedActors.clear();
	pickedComponent = nullptr;
	spawnSystem = nullptr;
	actorTemplateFilename.clear();
}

void HandleActorPicking()
{
	if (transformGizmo.CheckMouseOver() || Core::gameplayOn || WorldEditor::vertexPaintActive) 
	{
		return;
	}

	if (Input::GetMouseLeftUp())
	{
		HitResult hit;
		if (RaycastFromScreen(hit))
		{
			//@Todo: move all these 'placement' blocks into functions

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
				auto pickedActor = WorldEditor::GetPickedActor();
				if (pickedActor)
				{
					auto pickedActorTransform = pickedActor->GetTransform();
					pickedActor->Destroy();

					auto replacementActor = spawnSystem->SpawnActor(pickedActorTransform);
					replacementActor->Create();
					replacementActor->CreateAllComponents();

					editor->UpdateWorldList();

					WorldEditor::ClearPickedActors();
					WorldEditor::SetPickedActor(replacementActor);
				}
			}
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

				//@Todo: move to Paint debug menu
				auto newDuplicateActorMeshes = newDuplicateActor->GetComponentsOfType<MeshComponent>();
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
							actor->Destroy();
						}
					}
					else
					{
						debugMenu.AddNotification(VString::wformat(
							L"Destroyed actor [%S]", pickedActor->GetName().c_str()));
						pickedActor->Destroy();
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
				XMVECTOR spawnPos = XMLoadFloat3(&activeCamera->transform.position);
				const XMFLOAT3 forward = activeCamera->GetForwardVector();
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

	pickedActor = actor;
	editor->SetActorProps(pickedActor);
}

void VertexPainting()
{
	if (WorldEditor::vertexPaintActive)
	{
		if (Input::GetMouseLeftDown())
		{
			HitResult hit;
			if (RaycastFromScreen(hit))
			{
				auto meshes = hit.hitActor->GetComponentsOfType<MeshComponent>();
				for (auto mesh : meshes)
				{
					for (auto& vertIndex : hit.hitVertIndexes)
					{
						Vertex& v = mesh->meshDataProxy.vertices.at(vertIndex);
						v.colour = WorldEditor::vertexPaintColour;
					}

					mesh->CreateNewVertexBuffer();
				}
			}
		}
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

	debugMenu.AddNotification(VString::wformat(
		L"Spawned actor [%s] from template", actor->GetName().c_str()));	

	return actor;
}

