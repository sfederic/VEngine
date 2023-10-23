#include "vpch.h"
#include "Weight.h"
#include "Physics/Raycast.h"
#include "Core/VMath.h"
#include "Crushable.h"

void Weight::Create()
{
	moveSpeed = moveSpeed / 2.f;
	rotateSpeed = rotateSpeed / 2.f;
}

void Weight::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	CrushBreakableGridActorUnderneath();
}

void Weight::CrushBreakableGridActorUnderneath()
{
	HitResult hit(this);
	if (Raycast(hit, GetPositionV(), -VMath::GlobalUpVector(), 1.f))
	{
		auto crushable = dynamic_cast<Crushable*>(hit.hitActor);
		if (crushable)
		{
			crushable->Crushed();
		}
	}
}
