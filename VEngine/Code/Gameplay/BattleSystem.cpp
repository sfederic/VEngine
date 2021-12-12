#include "BattleSystem.h"
#include "World.h"
#include "Actors/Game/Unit.h"
#include "Actors/Game/Player.h"
#include "Actors/Game/Grid.h"
#include "Gameplay/GameUtils.h"
#include "Log.h"

BattleSystem battleSystem;

BattleSystem::BattleSystem() : System("BattleSystem")
{
}

void BattleSystem::StartBattle()
{
	if (isBattleActive) return;

	Log("Battle started.");

	isBattleActive = true;

	grid = GameUtils::GetGrid();
	grid->SetActive(true);

	player = GameUtils::GetPlayer();
	player->isPlayerTurn = true;

	activeBattleUnits = world.GetAllActorsOfTypeInWorld<Unit>();
	for (auto unit : activeBattleUnits)
	{
		unit->isInBattle = true;
	}
}

void BattleSystem::EndBattle()
{
	player->isWeaponDrawn = false;
	player->isPlayerTurn = false;
	player->RefreshCombatStats();
	
	grid->SetActive(false);

	Log("Battle ended.");

	isBattleActive = false;

	grid = nullptr;
	player = nullptr;
	activeBattleUnits.clear();
	currentUnitTurnIndex = 0;
}

void BattleSystem::MoveToNextTurn()
{
	if (!isBattleActive) return;

	//Now player's turn
	if (currentUnitTurnIndex >= activeBattleUnits.size())
	{
		Log("Players turn");
		player->isPlayerTurn = true;
		player->RefreshCombatStats();
		currentUnitTurnIndex = 0;
		return;
	}

	player->isPlayerTurn = false;

	//next enemy turn
	Log("Unit [%s] turn.", activeBattleUnits[currentUnitTurnIndex]->name.c_str());
	activeBattleUnits[currentUnitTurnIndex]->StartTurn();
	currentUnitTurnIndex++;
}

void BattleSystem::RemoveUnit(Unit* unit)
{
	activeBattleUnits.erase(std::remove(activeBattleUnits.begin(),
		activeBattleUnits.end(),
		unit),
		activeBattleUnits.end());

	if (CheckIfBattleIsOver())
	{
		EndBattle();
	}
}

bool BattleSystem::CheckIfBattleIsOver()
{
	return activeBattleUnits.size() == 0;
}
