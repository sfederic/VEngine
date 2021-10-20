#include "GameUtils.h"
#include "Actors/Player.h"
#include "Actors/BattleGrid.h"
#include "Audio/AudioSystem.h"
#include "World.h"
#include "FileSystem.h"
#include "Editor/Editor.h"

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

	void PlayAudio(const std::string audioFilename)
	{
		audioSystem.PlayAudio(audioFilename);
	}

	void SaveGameWorldState()
	{
		world.worldFilename += "01";
		fileSystem.WriteAllActorSystems();
		editor->Log("Game saved");
	}
}
