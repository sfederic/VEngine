#include "GameUtils.h"
#include "Actors/Player.h"
#include "Actors/BattleGrid.h"
#include "Audio/AudioSystem.h"

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

	BattleGrid* GetBattleGrid()
	{
		return BattleGrid::system.actors[0];
	}

	AudioBase* PlayAudio(const std::string filename)
	{
		auto audio = audioSystem.FindAudio(filename);
		audioSystem.PlayAudio(audio);
		return audio;
	}
}
