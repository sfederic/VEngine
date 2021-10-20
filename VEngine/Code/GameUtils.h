#pragma once
#include <string>

struct Player;
struct BattleGrid;

//Utility functions for in-game stuff
namespace GameUtils
{
	Player* GetPlayer();
	BattleGrid* GetBattleGrid();
	void PlayAudio(const std::string audioFilename);
	void SaveGameWorldState();
};
