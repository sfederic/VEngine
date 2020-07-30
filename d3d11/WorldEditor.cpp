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
		float dx = XMConvertToRadians(0.25f * (float)(x - lastMousePos.x));
		float dy = XMConvertToRadians(0.25f * (float)(y - lastMousePos.y));

		actor->Move(dx * pickedActorMoveSpeed, XMVectorSet(1.f, 0.f, 0.f, 0.f));
		actor->Move(-dy * pickedActorMoveSpeed, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	}

	lastMousePos.x = x;
	lastMousePos.y = y;
}
