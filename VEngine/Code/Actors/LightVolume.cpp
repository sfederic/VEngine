#include "vpch.h"
#include "LightVolume.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/MeshComponent.h"
#include "Components/Lights/PointLightComponent.h"

LightVolume::LightVolume()
{
    boxTrigger = CreateComponent(BoxTriggerComponent(), "BoxTrigger");
    boxTrigger->renderWireframeColour = XMFLOAT4(1.f, 1.f, 0.f, 1.f);
    rootComponent = boxTrigger;
}

Properties LightVolume::GetProps()
{
    auto props = __super::GetProps();
    props.title = "LightVolume";
    return props;
}

void LightVolume::EnableLightsIfInsideVolume()
{
    for (auto& pointLight : PointLightComponent::system.GetComponents())
    {
        if (boxTrigger->Contains(pointLight->GetWorldPositionV()))
        {
            pointLight->lightData.enabled = 1;
        }
        else
        {
            pointLight->lightData.enabled = 0;
        }
    }
}

void LightVolume::EnableAllLights()
{
    for (auto& pointLight : PointLightComponent::system.GetComponents())
    {
        pointLight->lightData.enabled = 1;
    }
}

bool LightVolume::IsMeshWithinVolume(MeshComponent* mesh)
{
    return boxTrigger->Contains(mesh->GetWorldPositionV());
}
