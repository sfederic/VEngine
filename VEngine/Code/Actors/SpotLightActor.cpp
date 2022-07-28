#include "vpch.h"
#include "SpotLightActor.h"
#include "Components/Lights/SpotLightComponent.h"

SpotLightActor::SpotLightActor()
{
    spotLight = SpotLightComponent::system.Add("SpotLight", this);
    rootComponent = spotLight;
}

Properties SpotLightActor::GetProps()
{
    Properties props = Actor::GetProps();
    props.title = "SpotLightActor";
    return props;
}
