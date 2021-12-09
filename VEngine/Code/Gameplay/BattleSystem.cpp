#include "BattleSystem.h"
#include "World.h"
#include "Actors/Game/Unit.h"
#include "Actors/Game/Player.h"
#include "Actors/Game/Grid.h"
#include "Gameplay/GameUtils.h"

BattleSystem battleSystem;

BattleSystem::BattleSystem() : System("BattleSystem")
{
}

void BattleSystem::StartBattle()
{
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

	grid = nullptr;
	player = nullptr;
	activeBattleUnits.clear();
	currentUnitTurnIndex = 0;
}

void BattleSystem::MoveToNextTurn()
{
	if (!player->inCombat) return;

	if (currentUnitTurnIndex >= activeBattleUnits.size())
	{
		player->isPlayerTurn = true;
		currentUnitTurnIndex = 0;
		return;
	}

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
