#include "vpch.h"
#include "SawBlade.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/MeshComponent.h"

SawBlade::SawBlade()
{
	sliceTrigger = CreateComponent<BoxTriggerComponent>("SliceTrigger");
	rootComponent->AddChild(sliceTrigger);
}

void SawBlade::Create()
{
	sliceTrigger->SetExtents(1.5f, 1.5f, 1.5f);

	SetMeshFilename("saw_blade.vmesh");
}

void SawBlade::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	SliceSurroundingGridActors();
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
	if (!isCutting)
	{
		return;
	}

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
