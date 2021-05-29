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


	//Actor picking for editor
	if (gInputSystem.GetMouseLeftDownState() && !gUISystem.bUIClicked)
	{
		lastMousePosX = gUISystem.mousePos.x;
		lastMousePosY = gUISystem.mousePos.y;

		Ray screenPickRay;

		if (RaycastAllFromScreen(screenPickRay, gUISystem.mousePos.x, gUISystem.mousePos.y, &editorCamera, GetWorld()))
		{
			//if (screenPickRay.hitActors.size() > 1) //Checking if multiple bounding volumes are hit
			{
				if (RaycastTriangleIntersect(screenPickRay))
				{
					if (debugLinesBuffer)
					{
						DrawRayDebug(screenPickRay.origin, screenPickRay.direction, screenPickRay.distance, debugLinesBuffer);
					}

					actorIndex = screenPickRay.actorIndex;
					actorSystemIndex = screenPickRay.actorSystemIndex;

					pickedActor = screenPickRay.hitActor;


					//Set Properties Dock Panel from actorsystem
					if (pickedActor)
					{
						ActorSystem* actorSystemToDisplay = GetWorld()->GetActorSystem(actorSystemIndex);
						gEditorSystem->DisplayActorSystemProperties(pickedActor);
					}
				}
			}
		}
	}
}

void WorldEditor::Init()
{

}

void WorldEditor::MoveActor(Actor* actor, XMVECTOR direction)
{
	int mouseX = gUISystem.mousePos.x;
	int mouseY = gUISystem.mousePos.y;

	if (gInputSystem.GetAsyncKey(Keys::LeftMouse))
	{
		SetCapture(gCoreSystem.mainWindow);

		float dx = XMConvertToRadians(0.25f * (float)(mouseX - lastMousePosX));
		float dy = XMConvertToRadians(0.25f * (float)(mouseY - lastMousePosY));

		if (actor)
		{
			XMVECTOR up = XMVectorEqual(direction, XMVectorUp());
			//if (up.m128_f32[0] == 0)
			{
				actor->Move((dy + dx) * pickedActorMoveSpeed, direction);
			}
		}

		ReleaseCapture();
	}

	lastMousePosX = mouseX;
	lastMousePosY = mouseY;
}

//Actors as the movement along the translation axis using mouse (Different from Actor::Move())
void WorldEditor::MoveActor(Actor* actor)
{
	int mouseX = gUISystem.mousePos.x;
	int mouseY = gUISystem.mousePos.y;

	if (gInputSystem.GetAsyncKey(Keys::LeftMouse))
	{
		SetCapture(gCoreSystem.mainWindow);

		float dx = XMConvertToRadians(0.25f * (float)(mouseX - lastMousePosX));
		float dy = XMConvertToRadians(0.25f * (float)(mouseY - lastMousePosY));

		ReleaseCapture();
	}

	lastMousePosX = mouseX;
	lastMousePosY = mouseY;
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
