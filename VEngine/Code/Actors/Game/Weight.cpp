#include "vpch.h"
#include "Weight.h"
#include "Physics/Raycast.h"
#include "Core/VMath.h"

void Weight::Tick(float deltaTime)
{
	__super::Tick(deltaTime);
}

void Weight::CrushBreakableGridActorUnderneath()
{
	HitResult hit(this);
	if (Raycast(hit, GetPositionV(), -VMath::GlobalUpVector(), 1.f))
	{
		//... breakable and whatever else actor checks
	}
}
