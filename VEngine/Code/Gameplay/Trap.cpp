#include "vpch.h"
#include "Trap.h"
#include "Actors/Game/Player.h"

void Trap::Set()
{
	auto player = Player::system.GetFirstActor();
	player->PlaceTrap(this);
}
