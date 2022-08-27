#include "vpch.h"
#include "CameraActor.h"
#include "Camera.h"

CameraActor::CameraActor()
{
	camera = CreateComponent(CameraComponent(), "Camera");
	rootComponent = camera;
}
