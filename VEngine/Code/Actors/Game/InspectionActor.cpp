#include "vpch.h"
#include "InspectionActor.h"
#include "Player.h"

void InspectionActor::Create()
{
	isInspectable = true;
}

void InspectionActor::Inspect()
{
	auto player = Player::system.GetOnlyActor();
	player->SetCameraTargetActor(this);
	auto nextCameraPos = XMVectorSet(0.f, 0.4f, 0.f, 1.f);
	player->SetNextCameraPosition(nextCameraPos);
}
