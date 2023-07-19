#include "vpch.h"
#include "Drill.h"
#include "Crushable.h"
#include "Physics/Raycast.h"
#include "Gameplay/GridNode.h"

void Drill::Create()
{
	SetMeshFilename("drill.vmesh");
}

void Drill::OnLinkMove()
{
	__super::OnLinkMove();

	HitResult hit(this);
	if (Raycast(hit, GetPositionV(), GetForwardVectorV(), 1.25f))
	{
		auto crushable = hit.GetHitActorAs<Crushable>();
		if (crushable)
		{
			//Reset hit actor's node else Drill will move up.
			auto node = crushable->GetCurrentNode();
			HitResult nodeHit(crushable);
			nodeHit.actorsToIgnore.push_back(this);
			node->RecalcNodeHeight(nodeHit);

			crushable->Crushed();
		}
	}
}
