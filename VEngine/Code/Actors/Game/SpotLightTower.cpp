#include "vpch.h"
#include "SpotLightTower.h"
#include "Components/Lights/SpotLightComponent.h"

SpotLightTower::SpotLightTower()
{
	spotLight = CreateComponent<SpotLightComponent>("SpotLight");
	rootComponent->AddChild(spotLight);
}

void SpotLightTower::Create()
{
	spotLight->SetLocalPosition(0.f, 1.f, 0.f);
}
