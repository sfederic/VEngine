#include "vpch.h"
#include "PointLightComponent.h"

void PointLightComponent::Create()
{
	layer = CollisionLayers::Editor;
	lightData.lightType = (int)LightType::Point;
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

Light PointLightComponent::GetLightData()
{
	XMStoreFloat4(&lightData.position, GetWorldPositionV());
	return lightData;
}
