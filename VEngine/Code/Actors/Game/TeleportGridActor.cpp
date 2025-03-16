
#include "TeleportGridActor.h"
import Physics.Raycast;

using namespace DirectX;

void TeleportGridActor::OnLinkMove()
{
	__super::OnLinkMove();

	HitResult hit(this);
	if (Physics::Raycast(hit, GetPositionV(), nextMoveCardinalDirection, 50.f))
	{
		auto hitPos = hit.GetHitPosV();
		hitPos -= nextMoveCardinalDirection / 2.f;
		SetNextPos(hitPos);
	}
	else
	{
		SetNextPos(GetPositionV());
	}
}
