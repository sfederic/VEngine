#include "DirectionalLightComponent.h"

void DirectionalLightComponent::Tick(double deltaTime)
{
}

void DirectionalLightComponent::Create()
{
}

Properties DirectionalLightComponent::GetProps()
{
    Properties props("DirectionalLight");
    props.Add("Colour", &colour);
    return props;
}
