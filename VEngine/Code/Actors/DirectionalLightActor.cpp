#include "DirectionalLightActor.h"
#include "Components/Lights/DirectionalLightComponent.h"

DirectionalLightActor::DirectionalLightActor()
{
	directionalLight = DirectionalLightComponent::system.Add(this);
	rootComponent = directionalLight;
}

Properties DirectionalLightActor::GetProps()
{
	auto props = Actor::GetProps();
	props.title = "DirectionalLightActor";
	return props;
}
