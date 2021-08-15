#include "WorldEditor.h"
#include "Physics/Raycast.h"
#include "Input.h"

WorldEditor worldEditor;

void WorldEditor::Tick()
{
	HandleActorPicking();
}

void WorldEditor::HandleActorPicking()
{
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
				}
			}
			else
			{
				pickedActor = screenPickRay.hitActor;
			}
		}
	}
}
