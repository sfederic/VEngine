#include "vpch.h"
#include "MapScreenSelector.h"
#include "Components/CameraComponent.h"

void MapScreenSelector::Create()
{
	camera = CreateComponent<CameraComponent>("Camera");
}

void MapScreenSelector::Tick(float deltaTime)
{
}

Properties MapScreenSelector::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
