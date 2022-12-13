#include "vpch.h"
#include "TrapCard.h"
#include "Actors/Game/Player.h"

void TrapCard::Set()
{
	auto player = Player::system.GetFirstActor();
	player->PlaceTrap(this);

	__super::Activate();
}
