#include "GameUtils.h"
#include "Actors/Player.h"

namespace GameUtils
{
	Player* GetPlayer()
	{
		return Player::system.actors[0];
	}
}
