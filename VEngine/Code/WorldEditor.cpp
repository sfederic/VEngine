#include "WorldEditor.h"
#include "Physics/Raycast.h"
#include "Input.h"
#include "Editor/Editor.h"
#include "Actors/Actor.h"
#include "Actors/IActorSystem.h"
#include "Editor/TransformGizmo.h"
#include "FileSystem.h"

WorldEditor worldEditor;

void WorldEditor::Tick()
{
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

	if (Input::GetKeyDown(Keys::LeftMouse))
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
		if (Input::GetKeyUp(Keys::D))
		{
			if (pickedActor)
			{
				pickedActor->actorSystem->SpawnActor(pickedActor->GetTransform());
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
		}
	}
}
