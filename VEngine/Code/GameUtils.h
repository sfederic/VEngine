#pragma once

struct Player;
struct BattleGrid;

//Utility functions for in-game stuff
namespace GameUtils
{
	Player* GetPlayer();
	BattleGrid* GetBattleGrid();
};
