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

	SetMeshFilename("crystal.vmesh");

	effectTrigger->SetExtents(0.5f, 0.5f, 0.5f);
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

		auto bounds = actor->GetRootComponent().GetBoundsInWorldSpace();
		if (effectTrigger->Intersects(bounds))
		{
			actor->PowerUp();
		}
		else
		{
			actor->PowerDown();
		}
	}
}
