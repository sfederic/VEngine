#include "vpch.h"
#include "DirectionalLightActor.h"
#include "Components/Lights/DirectionalLightComponent.h"

DirectionalLightActor::DirectionalLightActor()
{
	directionalLight = DirectionalLightComponent::system.Add("DirectionalLight", this);
	rootComponent = directionalLight;
}

Properties DirectionalLightActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
