#include "DirectionalLightActor.h"
#include "Components/Lights/DirectionalLightComponent.h"

DirectionalLightActor::DirectionalLightActor()
{
	directionalLight = DirectionalLightComponent::system.Add(this);
	rootComponent = directionalLight;
}

Properties DirectionalLightActor::GetProps()
{
	return Actor::GetProps();
}
