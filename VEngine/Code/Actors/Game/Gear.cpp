#include "vpch.h"
#include "Gear.h"
#include "Components/MeshComponent.h"
#include "GearSet.h"

void Gear::Create()
{
	ignoreRotationValidCheck = true;

	mesh->SetMeshFilename("gear.vmesh");
}

void Gear::OnLinkRotate()
{
	for (auto& gearSet : GearSet::system.GetActors())
	{
		if (gearSet->Intersects(mesh->GetBoundsInWorldSpace()))
		{
			gearSet->AddNextRotation(gearSet->GetUpVectorV(), 90.f);
			gearSet->IncrementDoor();
		}
	}
}
