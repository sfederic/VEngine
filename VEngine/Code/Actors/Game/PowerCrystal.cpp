
#include "PowerCrystal.h"
#include "Components/BoxTriggerComponent.h"

PowerCrystal::PowerCrystal()
{
	effectTrigger = CreateComponent<BoxTriggerComponent>("EffectTrigger");
	AddChildToRoot(effectTrigger);
}

void PowerCrystal::Create()
{
	__super::Create();

	SetMeshFilename("crystal.vmesh");

	effectTrigger->SetExtents(1.f, 1.f, 1.f);
}

void PowerCrystal::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	PowerUpSurroundingGridActors();
}

void PowerCrystal::PowerUpSurroundingGridActors()
{
	for (GridActor* actor : World::Get().GetAllActorsAsBaseType<GridActor>())
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
