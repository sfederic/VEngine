#include "vpch.h"
#include "Magnet.h"
#include "Physics/Raycast.h"

void Magnet::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	HitResult hit(this);
	if (Raycast(hit, GetPositionV(), GetForwardVectorV(), 10.f))
	{
		auto gridActor = hit.GetHitActorAs<GridActor>();
		if (gridActor && gridActor->CheckMovementAndRotationStopped())
		{
			gridActor->SetNextPos(GetPositionV());
		}
	}
}
