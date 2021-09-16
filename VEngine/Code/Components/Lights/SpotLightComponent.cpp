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
    props.Add("Const Atten", &lightData.constantAtten);
    props.Add("Linear Atten", &lightData.linearAtten);
    props.Add("Quad Atten", &lightData.quadraticAtten);
    props.Add("LightEnabled", &lightData.enabled);
    return props;
}
