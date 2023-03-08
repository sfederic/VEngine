#include "vpch.h"
#include "CameraActor.h"
#include "Components/CameraComponent.h"

CameraActor::CameraActor()
{
	camera = CreateComponent<CameraComponent>("Camera");
	rootComponent = camera;
}
