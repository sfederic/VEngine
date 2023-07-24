#include "vpch.h"
#include "SawBlade.h"
#include "Components/BoxTriggerComponent.h"

SawBlade::SawBlade()
{
	sliceTrigger = CreateComponent<BoxTriggerComponent>("SliceTrigger");
	rootComponent->AddChild(sliceTrigger);
}

void SawBlade::Create()
{
	SetMeshFilename("saw_blade.vmesh");
}

void SawBlade::PowerUp()
{
	isCutting = true;
	AddRotation(GetUpVectorV(), 50.f);
}

void SawBlade::PowerDown()
{
	isCutting = false;
}

void SawBlade::SliceSurroundingGridActors()
{
	for (GridActor* actor : World::GetAllActorsAsBaseType<GridActor>())
	{
		if (actor == this)
		{
			continue;
		}

		if (sliceTrigger->Contains(actor->GetPositionV()))
		{
			actor->Slice();
		}
	}
}
