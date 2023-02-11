#include "vpch.h"
#include "CameraActor.h"
#include "Core/Camera.h"

CameraActor::CameraActor()
{
	camera = CreateComponent<CameraComponent>("Camera");
	rootComponent = camera;
}
