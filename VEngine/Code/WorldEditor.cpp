#include "WorldEditor.h"
#include "Physics/Raycast.h"
#include "Input.h"
#include "Editor/Editor.h"
#include "Actors/Actor.h"
#include "Actors/IActorSystem.h"
#include "Editor/TransformGizmo.h"
#include "FileSystem.h"
#include "Camera.h"
#include "Actors/MeshActor.h"
#include "Editor/DebugMenu.h"
#include "Core.h"

WorldEditor worldEditor;

void WorldEditor::Tick()
{
	SpawnActorOnClick();
	HandleActorPicking();
	DuplicateActor();
	DeleteActor();

	SaveWorld();
}

void WorldEditor::HandleActorPicking()
{
	if (transformGizmo.CheckMouseOver() || Core::gameplayOn) 
	{
		return;
	}

	if (Input::GetMouseLeftDown())
	{
		Ray screenPickRay;
		if (RaycastFromScreen(screenPickRay))
		{
			if (Input::GetAsyncKey(Keys::Ctrl) || pickedActors.empty())
			{
				pickedActors.insert(screenPickRay.hitActor);
			}
			else
			{
				pickedActors.clear();
			}

			SetPickedActor(screenPickRay.hitActor);
		}
	}
}

void WorldEditor::DuplicateActor()
{
	if (Input::GetAsyncKey(Keys::Ctrl))
	{
		if (Input::GetKeyDown(Keys::W))
		{
			if (pickedActor)
			{
				pickedActors.clear();

				Transform transform = pickedActor->GetTransform();
				Actor* newDuplicateActor = pickedActor->actorSystem->SpawnActor(transform);

				//The props copying below will overwrite the new actor's name, so keep it here then copy it back.
				const std::string newActorOriginalName = newDuplicateActor->name;

				//Copy values across
				auto oldProps = pickedActor->GetAllProps();
				auto newProps = newDuplicateActor->GetAllProps();
				Properties::CopyProperties(oldProps, newProps);

				newDuplicateActor->CreateAllComponents();

				newDuplicateActor->name = newActorOriginalName;

				editor->SetActorProps(newDuplicateActor);
				editor->UpdateWorldList();

				//Set new actor as picked in-editor
				pickedActor = newDuplicateActor;
			}
		}
	}
}

void WorldEditor::SaveWorld()
{
	if (Input::GetAsyncKey(Keys::Ctrl))
	{
		if (Input::GetKeyUp(Keys::S))
		{
			fileSystem.WriteAllActorSystems();
		}
	}
}

void WorldEditor::DeleteActor()
{
	if (pickedActor)
	{
		if (Input::GetKeyUp(Keys::Delete))
		{
			editor->RemoveActorFromWorldList(nullptr);

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
				pickedActor->Destroy();
			}

			pickedActors.clear();
			pickedActor = nullptr;

			editor->ClearProperties();
		}
	}
}

//Spawn actor on middle mouse click in viewport
void WorldEditor::SpawnActorOnClick()
{
	if (Input::GetMouseMiddleUp())
	{
		if (spawnSystem)
		{
			Ray ray;
			if (RaycastFromScreen(ray))
			{
				//Spawn actor at ray hit point
				Transform transform;

				XMVECTOR dist = ray.direction * ray.hitDistance;
				XMVECTOR rayEnd = ray.origin + dist;

				//Round the position up for spawning on the grid in increments
				rayEnd = XMVectorRound(rayEnd);
				XMStoreFloat3(&transform.position, rayEnd);

				Actor* actor = spawnSystem->SpawnActor(transform);
				pickedActor = actor;
				editor->SetActorProps(pickedActor);
			}
			else
			{
				//Spawn actor a bit in front of the camera based on the click
				XMVECTOR spawnPos = XMLoadFloat3(&activeCamera->transform.position);
				XMFLOAT3 forward = activeCamera->GetForwardVector();
				XMVECTOR forwardVec = XMLoadFloat3(&forward);
				spawnPos += forwardVec * 10.0f;

				Transform transform;

				XMVECTOR dist = ray.direction * 10.f;
				XMVECTOR rayEnd = ray.origin + dist;

				//Round the position up for spawning on the grid in increments
				XMStoreFloat3(&transform.position, rayEnd);
				rayEnd = XMVectorRound(rayEnd);
				XMStoreFloat3(&transform.position, rayEnd);

				Actor* actor = spawnSystem->SpawnActor(transform);
				pickedActor = actor;
				editor->SetActorProps(pickedActor);
			}

			editor->UpdateWorldList();
		}
	}
}

void WorldEditor::DeselectPickedActor()
{
	pickedActor = nullptr;
	editor->ClearProperties();
}

void WorldEditor::SetPickedActor(Actor* actor)
{
	assert(actor);
	pickedActor = actor;

	pickedActors.insert(actor);

	editor->SetActorProps(pickedActor);
	editor->SelectActorInWorldList();
}
