
#include "MouseRotateComponent.h"

void MouseRotateComponent::Tick(float deltaTime)
{
	if (useMouseInput)
	{
		FPSCameraRotation();
	}
}
