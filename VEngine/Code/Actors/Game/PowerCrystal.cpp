#include "vpch.h"
#include "PowerCrystal.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/Lights/PointLightComponent.h"

PowerCrystal::PowerCrystal()
{
	pointLight = CreateComponent<PointLightComponent>("PointLight");
	rootComponent->AddChild(pointLight);

	effectTrigger = CreateComponent<BoxTriggerComponent>("EffectTrigger");
	rootComponent->AddChild(effectTrigger);
}

void PowerCrystal::Create()
{
	SetMeshFilename("crystal_light.vmesh");

	effectTrigger->SetExtents(2.f, 2.f, 2.f);
}

void PowerCrystal::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	PowerUpSurroundingGridActors();
}

void PowerCrystal::PowerUpSurroundingGridActors()
{
	for (auto& actor : World::GetAllActorsAsBaseType<GridActor>())
	{
		if (effectTrigger->Contains(actor->GetPositionV()))
		{
			actor->PowerUp();
		}
	}
}
