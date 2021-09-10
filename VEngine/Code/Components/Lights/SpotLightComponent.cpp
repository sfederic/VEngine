#include "SpotLightComponent.h"

SpotLightComponent::SpotLightComponent()
{
    lightData.lightType = (int)LightType::Spot;
}

void SpotLightComponent::Tick(double deltaTime)
{
}

void SpotLightComponent::Create()
{
}

Properties SpotLightComponent::GetProps()
{
    Properties props("SpotLightComponent");
    props.Add("Colour", &lightData.colour);
    props.Add("Spot Angle", &lightData.spotAngle);
    return props;
}
