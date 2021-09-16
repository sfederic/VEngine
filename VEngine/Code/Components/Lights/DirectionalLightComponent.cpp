#include "DirectionalLightComponent.h"

DirectionalLightComponent::DirectionalLightComponent()
{
    lightData.lightType = (int)LightType::Directional;
}

void DirectionalLightComponent::Tick(double deltaTime)
{
}

void DirectionalLightComponent::Create()
{
}

Properties DirectionalLightComponent::GetProps()
{
    Properties props("DirectionalLight");
    props.Add("Colour", &lightData.colour);
    props.Add("LightEnabled", &lightData.enabled);
    return props;
}
