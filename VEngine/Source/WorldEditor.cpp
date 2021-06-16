#include "WorldEditor.h"
#include "Input.h"
#include "Camera.h"
#include "UISystem.h"
#include "World.h"
#include "CoreSystem.h"
#include "ShaderFactory.h"
#include "MathHelpers.h"
#include "..\EditorMainWindow.h"
#include "PropertiesDock.h"
#include "EditorSystem.h"
#include "Actors/TestActor.h"
#include "ActorSystemFactory.h"
#include "DebugMenu.h"

WorldEditor gWorldEditor;

void WorldEditor::Tick(ID3D11Buffer* debugLinesBuffer)
{
	//Turn off currently selected actor spawn systsm.
	if (gInputSystem.GetKeyUpState(Keys::BackSpace))
	{
		ActorSystemFactory::SetCurrentActiveActorSystem(nullptr);
	}


	//Actor Hotkey commands
	if (pickedActor)
	{
		DeleteActor();
		DuplicateActor();
	}

	SpawnActorOnClick();

	//Actor picking for editor
	if (gInputSystem.GetMouseLeftDownState() && !bActorSpawnerOn)
	{
		lastMousePosX = gUISystem.mousePos.x;
		lastMousePosY = gUISystem.mousePos.y;

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

void WorldEditor::Init()
{

}

void WorldEditor::DeleteActor()
{
	if (gInputSystem.GetKeyUpState(Keys::Delete))
	{
		pickedActor->Destroy();
		pickedActor = nullptr;
	}
}

void WorldEditor::DuplicateActor()
{
	if (gInputSystem.GetAsyncKey(Keys::Ctrl))
	{
		if (gInputSystem.GetKeyUpState(Keys::D))
		{
			Transform transform = pickedActor->transform;
			Actor* newActor = pickedActor->linkedActorSystem->SpawnActor(transform);

			pickedActor = newActor;

			//Refresh world dock
			gEditorSystem->PopulateWorldList();
		}
	}

}

void WorldEditor::SetPickedActor(Actor* actor)
{
	pickedActor = actor;

	//Set Properties Dock Panel from actorsystem
	if (pickedActor)
	{
		ActorSystem* actorSystemToDisplay = pickedActor->linkedActorSystem;
		gEditorSystem->DisplayActorSystemProperties(pickedActor);
	}
}

void WorldEditor::SpawnActorOnClick()
{
	if (gInputSystem.GetKeyUpState(Keys::Tab))
	{
		bActorSpawnerOn = !bActorSpawnerOn;
		gDebugMenu.bActorSpawnMenuOpen = bActorSpawnerOn;
	}

	if (bActorSpawnerOn)
	{
		//Spawn actor on right click in viewport
		if (gInputSystem.GetMouseLeftUpState())
		{
			ActorSystem* actorSystem = ActorSystemFactory::GetCurrentActiveActorSystem();
			if (actorSystem)
			{
				GetWorld()->AddActorSystem(actorSystem); //Raycasts mess around a bit if world AS list is empty

				Ray ray;
				if (RaycastAllFromScreen(ray))
				{
					//Spawn actor at ray hit point
					Transform transform;

					XMVECTOR dist = ray.direction * ray.distance;
					XMVECTOR rayEnd = ray.origin + dist;
					XMStoreFloat3(&transform.position, rayEnd);

					actorSystem->SpawnActor(transform);
				}
				else
				{
					//Spawn actor a bit in front of the camera based on the click
					XMVECTOR spawnPos = GetActiveCamera()->location;
					spawnPos += GetActiveCamera()->forward * 10.0f;

					Transform transform;

					XMVECTOR dist = ray.direction * 10.f;
					XMVECTOR rayEnd = ray.origin + dist;
					XMStoreFloat3(&transform.position, rayEnd);

					actorSystem->SpawnActor(transform);
				}

				gEditorSystem->PopulateWorldList();
			}
		}
	}
}
