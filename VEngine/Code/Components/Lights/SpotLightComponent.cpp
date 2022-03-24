#include "SpotLightComponent.h"

SpotLightComponent::SpotLightComponent()
{
    layer = CollisionLayers::Editor;

    lightData.lightType = (int)LightType::Spot;
}

void SpotLightComponent::Tick(float deltaTime)
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
    props.Add("Intensity", &lightData.intensity);
    props.Add("LightEnabled", &lightData.enabled);
    return props;
}
