#include "vpch.h"
#include "Gear.h"
#include "Components/MeshComponent.h"
#include "GearSet.h"


void Gear::Create()
{
	mesh->SetMeshFilename("gear.vmesh");
}

void Gear::OnLinkRotate()
{
	for (auto& gearSet : GearSet::system.GetActors())
	{
		if (gearSet->Intersects(mesh->GetBoundsInWorldSpace()))
		{
			gearSet->AddRotation(gearSet->GetRightVectorV(), 90.f);
		}
	}
}
