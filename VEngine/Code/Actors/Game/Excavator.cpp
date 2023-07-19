#include "vpch.h"
#include "Excavator.h"
#include "Physics/Raycast.h"
#include "Mineable.h"
#include "Grid.h"

void Excavator::Create()
{
	canBeMovedInLink = false;
	canBeRotatedPitchXAxis = false;
	ignoreRotationValidCheck = true;

	SetMeshFilename("excavator.vmesh");
}

void Excavator::Interact()
{
	if (holdingObject)
	{
		ReleaseObject();
	}
	else
	{
		Dig();
	}
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
			holdingObject = true;
		}
	}
}

void Excavator::ReleaseObject()
{
	Transform transform = GetTransform();
	XMStoreFloat3(&transform.position, GetPositionV() + GetForwardVectorV());
	auto mineable = Mineable::system.Add(transform);
	mineable->Create();
	mineable->CreateAllComponents();
	mineable->Start();

	Grid::system.GetOnlyActor()->Awake();

	holdingObject = false;
}
