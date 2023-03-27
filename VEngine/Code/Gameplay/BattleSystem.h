#pragma once

#include <vector>

struct Unit;
class Player;
struct Grid;
struct PlayerActionBarWidget;

//Handles all units and player turns for battle as well as what units are active.
struct BattleSystem
{
	std::vector<Unit*> activeBattleUnits;

	bool isBattleActive = false;
	bool isPlayerTurn = true;

	int playerActionPoints = 10;
	PlayerActionBarWidget* actionBarWidget = nullptr;

private:
	Player* player = nullptr;
	Grid* grid = nullptr;

	int currentUnitTurnIndex = 0;

public:
	BattleSystem();
	void Reset();
	void StartBattle();
	void EndBattle();
	void MoveToNextTurn();
	void RemoveUnit(Unit* unit);
	bool CheckIfBattleIsOver();
};

extern BattleSystem battleSystem;
