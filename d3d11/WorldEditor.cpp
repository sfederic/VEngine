#include "WorldEditor.h"
#include "Input.h"
#include "Camera.h"
#include "UISystem.h"
#include "World.h"
#include "CoreSystem.h"

WorldEditor gWorldEditor;

void WorldEditor::Tick()
{
	//Actor picking for editor
	if (inputSystem.GetMouseLeftDownState())
	{
		if (RaycastAllFromScreen(screenPickRay, gUISystem.mousePos.x, gUISystem.mousePos.y, &editorCamera, GetWorld()))
		{
			actorIndex = screenPickRay.actorIndex;
			actorSystemIndex = screenPickRay.actorSystemIndex;
		}
	}
	else if (inputSystem.GetMouseRightUpState())
	{
		if (RaycastAllFromScreen(screenPickRay, gUISystem.mousePos.x, gUISystem.mousePos.y, &editorCamera, GetWorld()))
		{
			actorIndex = screenPickRay.actorIndex;
			actorSystemIndex = screenPickRay.actorSystemIndex;

			//Floating window for actor stats
			gUISystem.AddView(L"ActorTest", actorSystemIndex, actorIndex);
		}
	}
}

void WorldEditor::MoveActor(Actor* actor)
{
	static POINT lastMousePos;

	int x = gUISystem.mousePos.x;
	int y = gUISystem.mousePos.y;

	if (inputSystem.GetAsyncKey(VK_LBUTTON))
	{
		SetCapture(gCoreSystem.mainWindow);

		float dx = XMConvertToRadians(0.25f * (float)(x - lastMousePos.x));
		float dy = XMConvertToRadians(0.25f * (float)(y - lastMousePos.y));

		if (bMoveActorsInIncrements)
		{
			//For incremental movement
			dxAccum += dx;
			if (dxAccum > 0.5f)
			{
				actor->Move(moveIncrement, XMVectorSet(1.f, 0.f, 0.f, 0.f));
				dxAccum = 0;
			}
			else if (dxAccum < -0.5f)
			{
				actor->Move(-moveIncrement, XMVectorSet(1.f, 0.f, 0.f, 0.f));
				dxAccum = 0;
			}

			dyAccum += dy;
			if (dyAccum > 0.5f)
			{
				actor->Move(-moveIncrement, XMVectorSet(0.f, 1.f, 0.f, 0.f));
				dyAccum = 0;
			}
			else if (dyAccum < -0.5f)
			{
				actor->Move(moveIncrement, XMVectorSet(0.f, 1.f, 0.f, 0.f));
				dyAccum = 0;
			}
		}
		else
		{
			//For free movement
			actor->Move(dx * pickedActorMoveSpeed, XMVectorSet(1.f, 0.f, 0.f, 0.f));
			actor->Move(-dy * pickedActorMoveSpeed, XMVectorSet(0.f, 1.f, 0.f, 0.f));
		}

		ReleaseCapture();
	}

	lastMousePos.x = x;
	lastMousePos.y = y;
}
