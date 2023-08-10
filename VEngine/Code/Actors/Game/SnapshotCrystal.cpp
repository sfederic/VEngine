#include "vpch.h"
#include "SnapshotCrystal.h"
#include "Physics/Raycast.h"
#include "Components/MeshComponent.h"
#include "Core/Log.h"

void SnapshotCrystal::Create()
{
	__super::Create();

	projectionMesh->SetVisibility(false);
	projectionMesh->SetActive(false);
}

void SnapshotCrystal::Interact()
{
	SetProjectionMeshFromFacingGridActor();
}

void SnapshotCrystal::SetProjectionMeshFromFacingGridActor()
{
	HitResult hit(this);
	if (Raycast(hit, GetPositionV(), GetForwardVectorV(), 10.f))
	{
		auto gridActor = hit.GetHitActorAs<GridActor>();
		if (gridActor)
		{
			//Only get the first mesh
			int meshIndexCheck = 0;
			for (auto mesh : gridActor->GetComponentsOfType<MeshComponent>())
			{
				projectionMesh->SetMeshFilename(mesh->GetMeshFilename());
				projectionMesh->ReCreate();
				meshIndexCheck++;
			}

			//Log if there's more than one mesh set. 
			if (meshIndexCheck > 1)
			{
				Log("More than one mesh set for SnapshotCrystal [%s] on interact.", GetName().c_str());
			}
		}
	}
}
