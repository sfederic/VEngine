#include "GameUtils.h"
#include <filesystem>
#include "Actors/Game/Player.h"
#include "Actors/Game/BattleGrid.h"
#include "Audio/AudioSystem.h"
#include "World.h"
#include "FileSystem.h"

namespace GameUtils
{
	Player* GetPlayer()
	{
		if (!Player::system.actors.empty())
		{
			return Player::system.actors[0];
		}

		return nullptr;
	}

	BattleGrid* GetBattleGrid()
	{
		if (!BattleGrid::system.actors.empty())
		{
			return BattleGrid::system.actors[0];
		}

		return nullptr;
	}

	void PlayAudio(const std::string audioFilename)
	{
		audioSystem.PlayAudio(audioFilename);
	}

	void SaveGameWorldState()
	{
		//The deal with save files here is the the .vmap file is the original state of the world seen in-editor
		//and the .sav version of it is based on in-game player saves. So if a .sav version exists, that is loaded
		//instead of the .vmap file, during gameplay.

		auto firstOf = world.worldFilename.find_first_of(".");
		std::string str = world.worldFilename.substr(0, firstOf);
		std::string file = str += ".sav";

		world.worldFilename = file;
		fileSystem.WriteAllActorSystems();
	}

	void LoadGameWorldState(std::string worldName)
	{
		if (std::filesystem::exists("WorldMaps/" + worldName + ".sav"))
		{
			fileSystem.LoadWorld(worldName + ".sav");
		}
		else
		{
			fileSystem.LoadWorld(worldName + ".vmap");
		}
	}
}
