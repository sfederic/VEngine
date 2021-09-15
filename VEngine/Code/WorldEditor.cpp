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

WorldEditor worldEditor;

void WorldEditor::Tick()
{
	SpawnActorOnClick();
	DeselectPickedActor();
	HandleActorPicking();
	DuplicateActor();
	DeleteActor();

	SaveWorld();
}

void WorldEditor::HandleActorPicking()
{
	if (transformGizmo.CheckMouseOver()) 
	{
		return;
	}

	if (Input::GetMouseLeftDown())
	{
		Ray screenPickRay;

		if (RaycastAllFromScreen(screenPickRay))
		{
			if (screenPickRay.hitActors.size() > 1) //Checking if multiple bounding volumes are hit
			{
				if (RaycastTriangleIntersect(screenPickRay))
				{
					SetPickedActor(screenPickRay.hitActor);
				}
			}
			else
			{
				SetPickedActor(screenPickRay.hitActor);
			}
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
				Transform transform = pickedActor->GetTransform();
				Actor* newDuplicateActor = pickedActor->actorSystem->SpawnActor(transform);

				editor->UpdateWorldList();

				//Copy values across
				auto oldProps = pickedActor->GetAllProps();
				auto newProps = newDuplicateActor->GetAllProps();
				Properties::CopyProperties(oldProps, newProps);

				newDuplicateActor->CreateAllComponents();

				editor->SetActorProps(newDuplicateActor);

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
			pickedActor->Destroy();
			pickedActor = nullptr;

			editor->UpdateWorldList();
		}
	}
}

void WorldEditor::SpawnActorOnClick()
{
	//Spawn actor on right click in viewport
	if (Input::GetMouseMiddleUp())
	{
		if (spawnSystem)
		{
			Ray ray;
			if (RaycastAllFromScreen(ray))
			{
				//Spawn actor at ray hit point
				Transform transform;

				XMVECTOR dist = ray.direction * ray.distance;
				XMVECTOR rayEnd = ray.origin + dist;

				//Round the position up for spawning on the grid in increments
				rayEnd = XMVectorRound(rayEnd);
				XMStoreFloat3(&transform.position, rayEnd);

				Actor* actor = spawnSystem->SpawnActor(transform);
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
			}

			editor->UpdateWorldList();
		}
	}
}

void WorldEditor::DeselectPickedActor()
{
	if (Input::GetAsyncKey(Keys::ShiftLeft))
	{
		if (Input::GetKeyUp(Keys::C))
		{
			pickedActor = nullptr;
			editor->ClearProperties();
		}
	}
}

void WorldEditor::SetPickedActor(Actor* actor)
{
	assert(actor);
	pickedActor = actor;
	editor->SetActorProps(pickedActor);
}
