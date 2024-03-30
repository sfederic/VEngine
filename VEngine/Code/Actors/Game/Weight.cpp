#include "vpch.h"
#include "Weight.h"
#include "Physics/Raycast.h"
#include "Core/VMath.h"
#include "Crushable.h"

void Weight::Create()
{
	moveSpeed = 6.f;
	rotateSpeed = 6.f;
}

void Weight::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	CrushBreakableGridActorUnderneath();
}

void Weight::CrushBreakableGridActorUnderneath()
{
	HitResult hit(this);
	if (Physics::Raycast(hit, GetPositionV(), -VMath::GlobalUpVector(), 2.f))
	{
		auto crushable = hit.GetHitActorAs<Crushable>();
		if (crushable)
		{
			crushable->Crushed();
		}
	}
}
