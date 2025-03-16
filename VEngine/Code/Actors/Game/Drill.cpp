
#include "Drill.h"
import Physics.Raycast;

void Drill::Create()
{
	__super::Create();

	SetMeshFilename("drill.vmesh");
}

void Drill::OnLinkRotate()
{
	__super::OnLinkRotate();

	rotateDrillIncrement++;

	constexpr int maxDrillIncrement = 5;
	if (rotateDrillIncrement > maxDrillIncrement)
	{
		DrillThroughBelowGridActor();
	}
}

void Drill::DrillThroughBelowGridActor()
{
	HitResult hit(this);
	if (Physics::Raycast(hit, GetPositionV(), GetForwardVectorV(), 1.5f))
	{
		auto actor = hit.GetHitActorAs<GridActor>();
		if (actor)
		{
			actor->DeferDestroy();
		}
	}
}
