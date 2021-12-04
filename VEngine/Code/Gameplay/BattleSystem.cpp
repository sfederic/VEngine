#include "BattleSystem.h"
#include "World.h"
#include "Actors/Game/Unit.h"
#include "Actors/Game/Player.h"
#include "Gameplay/GameUtils.h"

BattleSystem battleSystem;

BattleSystem::BattleSystem() : System("BattleSystem")
{
}

void BattleSystem::StartBattle()
{
	activeBattleUnits = world.GetAllActorsOfTypeInWorld<Unit>();
	player = GameUtils::GetPlayer();
}
