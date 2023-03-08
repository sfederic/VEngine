#include "vpch.h"
#include "PointLightComponent.h"

void PointLightComponent::Create()
{
	layer = CollisionLayers::Editor;
	lightData.lightType = (int)LightType::Point;
}

void PointLightComponent::Tick(float deltaTime)
{
	XMStoreFloat4(&lightData.position, GetWorldPositionV());
}

Properties PointLightComponent::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Colour", &lightData.colour);
	props.Add("Intensity", &lightData.intensity);
	props.Add("LightEnabled", &lightData.enabled);
	return props;
}
