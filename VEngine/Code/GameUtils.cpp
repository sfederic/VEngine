#include "GameUtils.h"
#include "Actors/Player.h"

namespace GameUtils
{
	Player* GetPlayer()
	{
		if (Player::system.actors.size() > 0)
		{
			return Player::system.actors[0];
		}

		return nullptr;
	}
}
