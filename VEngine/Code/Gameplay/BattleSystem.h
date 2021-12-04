#pragma once
#include "System.h"
#include <vector>

struct Unit;
struct Player;
struct BattleGrid;

//Handles all units and player turns for battle as well as what units are active.
struct BattleSystem : System
{
	std::vector<Unit*> activeBattleUnits;

private:
	Player* player = nullptr;
	BattleGrid* battleGrid = nullptr;

	int currentUnitTurnIndex = 0;

public:
	BattleSystem();
	void StartBattle();
	void EndBattle();
	void MoveToNextTurn();
	void RemoveUnit(Unit* unit);
	bool CheckIfBattleIsOver();
};

extern BattleSystem battleSystem;
