#include "vpch.h"
#include "SpotLightComponent.h"

void SpotLightComponent::Create()
{
    layer = CollisionLayers::Editor;
    lightData.lightType = (int)LightType::Spot;
}

void SpotLightComponent::Tick(float deltaTime)
{

}

Properties SpotLightComponent::GetProps()
{
    auto props = __super::GetProps();
    props.title = "SpotLight";
    props.Add("Colour", &lightData.colour);
    props.Add("Spot Angle", &lightData.spotAngle);
    props.Add("Intensity", &lightData.intensity);
    props.Add("LightEnabled", &lightData.enabled);
    return props;
}
