#pragma once
#include <string>

struct Player;
struct BattleGrid;
struct AudioBase;

//Utility functions for in-game stuff
namespace GameUtils
{
	Player* GetPlayer();
	BattleGrid* GetBattleGrid();

	AudioBase* PlayAudio(const std::string filename);
};
