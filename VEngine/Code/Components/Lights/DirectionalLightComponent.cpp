#include "vpch.h"
#include "DirectionalLightComponent.h"

void DirectionalLightComponent::Create()
{
    shadowMapOrthoSize = 30.f;
    layer = CollisionLayers::Editor;
    lightData.lightType = (int)LightType::Directional;
}

void DirectionalLightComponent::Tick(float deltaTime)
{
    XMStoreFloat4(&lightData.position, GetWorldPositionV());
    XMStoreFloat4(&lightData.direction, GetForwardVectorV());
}

Properties DirectionalLightComponent::GetProps()
{
    auto props = __super::GetProps();
    props.title = "DirectionalLight";
    props.Add("Colour", &lightData.colour);
    props.Add("LightEnabled", &lightData.enabled);
    return props;
}
