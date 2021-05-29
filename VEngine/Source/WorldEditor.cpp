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

WorldEditor gWorldEditor;

void WorldEditor::Tick(ID3D11Buffer* debugLinesBuffer)
{
	if (gUISystem.bUIClicked)
	{
		return;
	}

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
	if (gInputSystem.GetMouseLeftDownState() && !gUISystem.bUIClicked)
	{
		lastMousePosX = gUISystem.mousePos.x;
		lastMousePosY = gUISystem.mousePos.y;

		Ray screenPickRay;

		if (RaycastAllFromScreen(screenPickRay, gUISystem.mousePos.x, gUISystem.mousePos.y, &editorCamera, GetWorld()))
		{
			if (screenPickRay.hitActors.size() > 1) //Checking if multiple bounding volumes are hit
			{
				if (RaycastTriangleIntersect(screenPickRay))
				{
					if (debugLinesBuffer)
					{
						DrawRayDebug(screenPickRay.origin, screenPickRay.direction, screenPickRay.distance, debugLinesBuffer);
					}

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
		if (gInputSystem.GetKeyDownState(Keys::D))
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
	//Spawn actor on right click in viewport
	if (gInputSystem.GetMouseMiddleUpState())
	{
		ActorSystem* actorSystem = ActorSystemFactory::GetCurrentActiveActorSystem();
		if (actorSystem)
		{
			XMVECTOR spawnPos = GetActiveCamera()->location;
			spawnPos += GetActiveCamera()->forward * 5.0f;

			Transform transform;
			XMStoreFloat3(&transform.position, spawnPos);

			actorSystem->SpawnActor(transform);
		}
	}
}
