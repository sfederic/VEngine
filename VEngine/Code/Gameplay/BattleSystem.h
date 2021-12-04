#pragma once
#include "System.h"
#include <vector>

struct Unit;
struct Player;

//Handles all units and player turns for battle as well as what units are active.
struct BattleSystem : System
{
	std::vector<Unit*> activeBattleUnits;
	Player* player = nullptr;

	BattleSystem();
	void StartBattle();
};

extern BattleSystem battleSystem;
