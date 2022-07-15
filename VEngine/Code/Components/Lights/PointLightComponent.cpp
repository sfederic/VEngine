#include "vpch.h"
#include "PointLightComponent.h"

PointLightComponent::PointLightComponent()
{
	layer = CollisionLayers::Editor;

	lightData.lightType = (int)LightType::Point;
}

void PointLightComponent::Tick(float deltaTime)
{
}

void PointLightComponent::Create()
{
}

Properties PointLightComponent::GetProps()
{
	auto props = __super::GetProps();
	props.title = "PointLight";
	props.Add("Colour", &lightData.colour);
	props.Add("Intensity", &lightData.intensity);
	props.Add("LightEnabled", &lightData.enabled);
	return props;
}
