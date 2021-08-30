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
					pickedActor = screenPickRay.hitActor;
					editor->ActorProps(pickedActor);
				}
			}
			else
			{
				pickedActor = screenPickRay.hitActor;
				editor->ActorProps(pickedActor);
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
			fileSystem.WriteAllActorSystems("test");
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
	if (Input::GetMouseRightUp())
	{
		IActorSystem* actorSystem = editor->spawnSystem;
		if (actorSystem)
		{
			Ray ray;
			if (RaycastAllFromScreen(ray))
			{
				//Spawn actor at ray hit point
				Transform transform;

				XMVECTOR dist = ray.direction * ray.distance;
				XMVECTOR rayEnd = ray.origin + dist;
				XMStoreFloat3(&transform.position, rayEnd);

				//actorSystem->SpawnActor(transform);
				MeshActor::system.Add(MeshActor(MeshActor::spawnMeshFilename), transform);
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
				XMStoreFloat3(&transform.position, rayEnd);

				//actorSystem->SpawnActor(transform);
				MeshActor::system.Add(MeshActor(MeshActor::spawnMeshFilename), transform);
			}

			editor->UpdateWorldList();
		}
	}
}
