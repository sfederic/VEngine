#include "vpch.h"
#include "PointLightActor.h"
#include "Components/Lights/PointLightComponent.h"

PointLightActor::PointLightActor()
{
	pointLight = PointLightComponent::system.Add("PointLight", this);
	rootComponent = pointLight;
}

Properties PointLightActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
