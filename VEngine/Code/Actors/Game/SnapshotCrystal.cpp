#include "vpch.h"
#include "SnapshotCrystal.h"
#include "Physics/Raycast.h"
#include "Components/MeshComponent.h"
#include "Core/Log.h"
#include "Gameplay/GameUtils.h"

void SnapshotCrystal::Create()
{
	__super::Create();

	projectionMesh->SetVisibility(false);
	projectionMesh->SetActive(false);

	interactText = L"Create Snapshot";
}

void SnapshotCrystal::Interact()
{
	projectionMesh->ToggleActive();
	projectionMesh->ToggleVisibility();

	SetProjectionMeshFromFacingGridActor();

	//Keep the parent Interact() below the projection mesh sets so that the Grid reset can work with that mesh.
	__super::Interact();
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
			for (auto mesh : gridActor->GetComponents<MeshComponent>())
			{
				projectionMesh->SetMeshFilename(mesh->GetMeshFilename());
				GameUtils::SetLinkedMeshEffect(projectionMesh);
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
