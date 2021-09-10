#include "PointLightComponent.h"

PointLightComponent::PointLightComponent()
{
	lightData.lightType = (int)LightType::Point;
}

void PointLightComponent::Tick(double deltaTime)
{
}

void PointLightComponent::Create()
{
}

Properties PointLightComponent::GetProps()
{
	Properties props("PointLightComponent");
	props.Add("Colour", &lightData.colour);
	props.Add("Const Atten.", &lightData.constantAtten);
	props.Add("Linear Atten.", &lightData.linearAtten);
	props.Add("Quad Atten.", &lightData.quadraticAtten);
	return props;
}
