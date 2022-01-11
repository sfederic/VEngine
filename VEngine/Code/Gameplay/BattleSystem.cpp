#include "BattleSystem.h"
#include "World.h"
#include "Actors/Game/Unit.h"
#include "Actors/Game/Player.h"
#include "Actors/Game/Grid.h"
#include "Actors/Game/NPC.h"
#include "Gameplay/GameUtils.h"
#include "Log.h"
#include "UI/UISystem.h"
#include "UI/HealthWidget.h"
#include "UI/DialogueWidget.h"
#include "UI/UnitLineupWidget.h"

BattleSystem battleSystem;

BattleSystem::BattleSystem() : System("BattleSystem")
{
}

void BattleSystem::Reset()
{
	isBattleActive = false;

	grid = nullptr;
	player = nullptr;

	activeBattleUnits.clear();

	currentUnitTurnIndex = 0;
}

void BattleSystem::StartBattle()
{
	if (isBattleActive) return;

	Log("Battle started.");

	isBattleActive = true;

	grid = GameUtils::GetGrid();
	grid->lerpValue = Grid::LerpValue::LerpOut;
	grid->SetActive(true);
	grid->ResetAllNodes();

	player = GameUtils::GetPlayer();
	player->isPlayerTurn = true;
	player->isWeaponDrawn = true;

	uiSystem.unitLineupWidget->AddToViewport();

	activeBattleUnits = world.GetAllActorsOfTypeInWorld<Unit>();
	for (auto unit : activeBattleUnits)
	{
		unit->isInBattle = true;
	}

	auto activeBattleNPCs = world.GetAllActorsOfTypeInWorld<NPC>();
	for (auto npc : activeBattleNPCs)
	{
		npc->BattleStartDialogue();
	}

	auto healthWidgets = uiSystem.GetAllWidgetsOfType<HealthWidget>();
	for (auto healthWidget : healthWidgets)
	{
		healthWidget->AddToViewport();
	}
}

void BattleSystem::EndBattle()
{
	player->BattleCleanup();

	grid->SetActive(false);

	uiSystem.unitLineupWidget->RemoveFromViewport();

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

		GameUtils::SetActiveCameraTarget(player);
		GameUtils::GetGrid()->ResetAllNodes();

		return;
	}

	player->isPlayerTurn = false;

	//next enemy turn
	auto unit = activeBattleUnits[currentUnitTurnIndex];
	Log("Unit [%s] turn.", unit->name.c_str());
	activeBattleUnits[currentUnitTurnIndex]->StartTurn();
	currentUnitTurnIndex++;

	GameUtils::SetActiveCameraTarget(unit);
	GameUtils::GetGrid()->ResetAllNodes();
}

void BattleSystem::RemoveUnit(Unit* unit)
{
	activeBattleUnits.erase(std::remove(activeBattleUnits.begin(),
		activeBattleUnits.end(),
		unit),
		activeBattleUnits.end());

	//Show death text.
	if (!unit->deathText.empty())
	{
		//@Todo: currently the death text doesn't hold on the actors position
		auto deathDialogueWidget = CreateWidget<DialogueWidget>();
		deathDialogueWidget->pos = unit->GetPositionVector();
		deathDialogueWidget->SetText(unit->deathText);
		deathDialogueWidget->AddToViewport(5.0f);
	}

	if (CheckIfBattleIsOver())
	{
		EndBattle();
	}
}

bool BattleSystem::CheckIfBattleIsOver()
{
	return activeBattleUnits.size() == 0;
}
