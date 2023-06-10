#include "vpch.h"
#include "Excavator.h"
#include "Components/MeshComponent.h"
#include "Physics/Raycast.h"
#include "Mineable.h"

void Excavator::Create()
{
	canBeMovedInLink = false;

	mesh->SetMeshFilename("excavator.vmesh");
}

void Excavator::Interact()
{
	Dig();
}

void Excavator::Dig()
{
	HitResult hit(this);
	const XMVECTOR center = GetPositionV() + GetForwardVectorV();
	SimpleBoxCast(center, XMFLOAT3(0.5f, 0.5f, 0.5f), hit, false, false);
	for (auto actor : hit.hitActors)
	{
		auto mineable = dynamic_cast<Mineable*>(actor);
		if(mineable)
		{
			mineable->Mine();
		}
	}
}
