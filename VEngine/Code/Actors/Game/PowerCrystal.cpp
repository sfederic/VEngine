#include "vpch.h"
#include "PowerCrystal.h"
#include "Components/BoxTriggerComponent.h"

PowerCrystal::PowerCrystal()
{
	effectTrigger = CreateComponent<BoxTriggerComponent>("EffectTrigger");
	rootComponent->AddChild(effectTrigger);
}

void PowerCrystal::Create()
{
	__super::Create();

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
	for (GridActor* actor : World::GetAllActorsAsBaseType<GridActor>())
	{
		if (actor == this)
		{
			continue;
		}

		if (effectTrigger->Contains(actor->GetPositionV()))
		{
			actor->PowerUp();
		}
		else
		{
			actor->PowerDown();
		}
	}
}
