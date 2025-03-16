
#include "PointLightGridActor.h"
#include "Components/Lights/PointLightComponent.h"

PointLightGridActor::PointLightGridActor()
{
	pointLight = CreateComponent<PointLightComponent>("PointLight");
	AddChildToRoot(pointLight);
}
