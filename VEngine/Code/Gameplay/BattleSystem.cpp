#include "vpch.h"
#include "BattleSystem.h"
#include "Core/World.h"
#include "Core/Log.h"
#include "Actors/Game/Unit.h"
#include "Actors/Game/Player.h"
#include "Actors/Game/Grid.h"
#include "Actors/Game/NPC.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/PlayerInputController.h"
#include "UI/UISystem.h"
#include "UI/Game/HealthWidget.h"
#include "UI/Game/DialogueWidget.h"
#include "UI/Game/UnitLineupWidget.h"
#include "UI/Game/PlayerHealthWidget.h"

BattleSystem battleSystem;

BattleSystem::BattleSystem()
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

	grid = Grid::system.GetFirstActor();
	grid->lerpValue = Grid::LerpValue::LerpOut;
	grid->SetActive(true);
	grid->ResetAllNodes();

	player = Player::system.GetFirstActor();
	player->SetupForBattle();

	UISystem::unitLineupWidget->AddToViewport();

	activeBattleUnits = World::GetAllActorsOfTypeInWorld<Unit>();
	for (auto unit : activeBattleUnits)
	{
		unit->isInBattle = true;
	}

	auto activeBattleNPCs = World::GetAllActorsOfTypeInWorld<NPC>();
	for (auto npc : activeBattleNPCs)
	{
		npc->BattleStartDialogue();
	}

	auto healthWidgets = UISystem::GetAllWidgetsOfType<HealthWidget>();
	for (auto healthWidget : healthWidgets)
	{
		healthWidget->AddToViewport();
	}
}

void BattleSystem::EndBattle()
{
	player->BattleCleanup();

	grid->SetActive(false);

	UISystem::unitLineupWidget->RemoveFromViewport();

	Log("Battle ended.");

	isBattleActive = false;

	grid->ResetAllNodes();
	grid->DisplayHideAllNodes();
	grid->DisarmAllTrapNodes();
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

		battleSystem.isPlayerTurn = true;

		player->RefreshCombatStats();
		player->DrawTurnBattleCardHand();

		currentUnitTurnIndex = 0;

		GameUtils::SetActiveCameraTarget(player);
		Grid::system.GetFirstActor()->ResetAllNodes();

		playerInputController.SetPlayerUnitToControl(player);

		return;
	}

	battleSystem.isPlayerTurn = false;

	//next enemy turn
	auto unit = activeBattleUnits[currentUnitTurnIndex];
	Log("Unit [%s] turn.", unit->GetName().c_str());
	activeBattleUnits[currentUnitTurnIndex]->StartTurn();
	currentUnitTurnIndex++;

	GameUtils::SetActiveCameraTarget(unit);
	Grid::system.GetFirstActor()->ResetAllNodes();
}

void BattleSystem::RemoveUnit(Unit* unit)
{
	activeBattleUnits.erase(std::remove(activeBattleUnits.begin(),
		activeBattleUnits.end(),
		unit),
		activeBattleUnits.end());

	//Show death text.
	if (!unit->deathText.empty() && unit->health <= 0)
	{
		//@Todo: currently the death text doesn't hold on the actors position
		auto deathDialogueWidget = UISystem::CreateWidget<DialogueWidget>();
		deathDialogueWidget->worldPosition = unit->GetHomogeneousPositionV();
		deathDialogueWidget->SetText(unit->deathText);
		deathDialogueWidget->AddToViewport(5.0f);
	}

	//Reset camera to player
	GameUtils::SetActiveCameraTarget(Player::system.GetFirstActor());

	if (CheckIfBattleIsOver())
	{
		EndBattle();
	}
}

bool BattleSystem::CheckIfBattleIsOver()
{
	return activeBattleUnits.size() == 0;
}
