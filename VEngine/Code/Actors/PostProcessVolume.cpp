#include "vpch.h"
#include "PostProcessVolume.h"
#include "Core/Camera.h"
#include "Components/BoxTriggerComponent.h"

PostProcessVolume::PostProcessVolume()
{
    boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
    boxTrigger->renderWireframeColour = XMFLOAT4(0.45f, 0.2f, 0.64f, 1.f);
    rootComponent = boxTrigger;
}

Properties PostProcessVolume::GetProps()
{
    Properties props = __super::GetProps();
    props.title = GetTypeName();
    props.Add("Exposure", &postProcessData.exposure);
    props.Add("Shoulder Str.", &postProcessData.shoulderStrength);
    props.Add("Linear Str.", &postProcessData.linearStrength);
    props.Add("Linear Angle", &postProcessData.linearAngle);
    props.Add("Toe Str.", &postProcessData.toeStrenth);
    props.Add("Toe Numerator", &postProcessData.toeNumerator);
    props.Add("Toe Denominator", &postProcessData.toeDenominator);
    props.Add("White Value", &postProcessData.linearWhitePointValue);
    return props;
}

bool PostProcessVolume::IsActiveCameraInsideVolume()
{
    CameraComponent* camera = activeCamera;
    if (camera)
    {
        XMVECTOR cameraWorldPos = camera->GetWorldPositionV();
        if (boxTrigger->Contains(cameraWorldPos))
        {
            return true;
        }
    }

    return false;
}
