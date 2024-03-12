#include "vpch.h"
#include "CameraActor.h"
#include "Components/CameraComponent.h"

CameraActor::CameraActor()
{
	camera = CreateComponent<CameraComponent>("Camera");
	rootComponent = camera;
}

Properties CameraActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
