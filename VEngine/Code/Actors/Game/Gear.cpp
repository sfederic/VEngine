#include "vpch.h"
#include "Gear.h"
#include "GearSet.h"
#include "Components/MeshComponent.h"

void Gear::Create()
{
	ignoreRotationValidCheck = true;

	SetMeshFilename("gear.vmesh");
}

void Gear::OnLinkRotate()
{
	__super::OnLinkRotate();

	for (auto& gearSet : GearSet::system.GetActors())
	{
		if (gearSet->Intersects(mesh->GetBoundsInWorldSpace()))
		{
			gearSet->AddNextRotation(gearSet->GetUpVectorV(), 90.f);
			gearSet->IncrementDoor();
		}
	}
}
