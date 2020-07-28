#include "WorldEditor.h"
#include "Input.h"
#include "Camera.h"
#include "UISystem.h"
#include "World.h"

WorldEditor gWorldEditor;

void WorldEditor::Tick()
{
	//Actor picking for editor
	if (inputSystem.GetMouseLeftUpState())
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