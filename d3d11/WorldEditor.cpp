#include "WorldEditor.h"
#include "Input.h"
#include "Camera.h"
#include "UISystem.h"
#include "World.h"

WorldEditor gWorldEditor;

void WorldEditor::Tick()
{
	if (inputSystem.GetMouseLeftUpState())
	{
		if (RaycastAllFromScreen(screenPickRay, uiSystem.mousePos.x, uiSystem.mousePos.y, &editorCamera, GetWorld()))
		{
			actorIndex = screenPickRay.actorIndex;
			actorSystemIndex = screenPickRay.actorSystemIndex;
		}
	}
}