
#include "SpotLightActor.h"
#include "Components/Lights/SpotLightComponent.h"

SpotLightActor::SpotLightActor()
{
	spotLight = CreateComponent<SpotLightComponent>("SpotLight");
	SetRootComponent(spotLight);
}

Properties SpotLightActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
