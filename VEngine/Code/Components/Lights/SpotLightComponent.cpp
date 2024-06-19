#include "vpch.h"
#include "SpotLightComponent.h"

void SpotLightComponent::Create()
{
	layer = CollisionLayers::Editor;
	lightData.lightType = (int)LightType::Spot;
}

Properties SpotLightComponent::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Colour", &lightData.colour);
	props.Add("Spot Angle", &lightData.spotAngle);
	props.Add("Intensity", &lightData.intensity);
	props.Add("LightEnabled", &lightData.enabled);
	props.Add("Shadows Enabled", &enabledShadows);
	return props;
}

LightData SpotLightComponent::GetLightData()
{
	XMStoreFloat4(&lightData.position, GetWorldPositionV());
	XMStoreFloat4(&lightData.direction, GetForwardVectorV());
	return lightData;
}
