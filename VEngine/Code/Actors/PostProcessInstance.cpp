#include "vpch.h"
#include "PostProcessInstance.h"
#include "Components/EmptyComponent.h"
#include "Render/RenderTypes.h"

PostProcessInstance::PostProcessInstance()
{
    rootComponent = EmptyComponent::system.Add(this);
}

Properties PostProcessInstance::GetProps()
{
    auto props = __super::GetProps();
    props.title = "PostProcessInstance";
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
