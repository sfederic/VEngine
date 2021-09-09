#include "PointLightComponent.h"

PointLightComponent::PointLightComponent()
{
	lightData.lightType = LightType::Point;
}

void PointLightComponent::Tick(double deltaTime)
{
}

void PointLightComponent::Create()
{
}

Properties PointLightComponent::GetProps()
{
	Properties props;
	props.Add("Range", &lightData.range);
	return props;
}
