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
	auto props = Actor::GetProps();
	props.title = "DirectionalLightActor";
	return props;
}
