#include "vpch.h"
#include "PostProcessVolume.h"

PostProcessVolume::PostProcessVolume()
{
    SetEmptyRootComponent();
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
