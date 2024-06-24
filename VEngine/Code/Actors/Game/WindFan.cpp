#include "vpch.h"
#include "WindFan.h"
#include "Physics/Raycast.h"

void WindFan::Create()
{
	__super::Create();
}

void WindFan::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	//Todo: this raycast might need more work because the actor that the WindFan pushes will have to get 
	//stuck on any actor that it collides with while moving.

	HitResult windHit(this);
	if (Physics::Raycast(windHit, GetPositionV(), GetForwardVectorV(), windRange))
	{
		const auto gridActor = windHit.GetHitActorAs<GridActor>();
		if (gridActor)
		{
			if (gridActor->CheckMovementAndRotationStopped())
			{
				const auto nextWindPushPos = gridActor->GetPositionV() + (GetForwardVectorV() * windRange);
				gridActor->SetNextPos(nextWindPushPos);

				//Todo: this recalc is also needed when the grid actor finishes its movement. Maybe just delete this whole actor, it's too hard to implement with the current systems.
				gridActor->RecalcCurrentNodePosition();
			}
		}
	}
}

Properties WindFan::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Wind Range", &windRange);
	return props;
}
