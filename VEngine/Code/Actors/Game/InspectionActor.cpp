#include "vpch.h"
#include "InspectionActor.h"
#include "Player.h"

void InspectionActor::Interact()
{
	auto player = Player::system.GetOnlyActor();
	player->ResetCameraPosAndTargetToPlayer();
	auto nextCameraPos = XMVectorSet(0.f, 0.5f, 0.f, 1.f);
	player->SetNextCameraPosition(nextCameraPos);
}
