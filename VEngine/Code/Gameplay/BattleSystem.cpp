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
	player = GameUtils::GetPlayer();

	activeBattleUnits = world.GetAllActorsOfTypeInWorld<Unit>();
	for (auto unit : activeBattleUnits)
	{
		unit->isInBattle = true;
	}
}

void BattleSystem::EndBattle()
{
	player->inCombat = false;

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

	if (currentUnitTurnIndex >= activeBattleUnits.size())
	{
		Log("Players turn");
		player->isPlayerTurn = true;
		currentUnitTurnIndex = 0;
		return;
	}

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
