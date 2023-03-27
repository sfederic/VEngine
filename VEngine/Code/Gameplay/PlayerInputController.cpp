#include "vpch.h"
#include "PlayerInputController.h"
#include "Core/Input.h"
#include "Core/Core.h"
#include "Components/CameraComponent.h"
#include "Actors/Game/PlayerUnit.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/BattleSystem.h"

PlayerInputController playerInputController;

void PlayerInputController::Tick(float deltaTime)
{
	if (Core::gameplayOn)
	{
		if (playerUnitToControl)
		{
			if (battleSystem.isBattleActive && !battleSystem.isPlayerTurn)
			{
				return;
			}

			playerUnitToControl->ControllerInput(deltaTime);

			if (playerUnitToControl->isMainPlayer)
			{
				playerUnitToControl->ToggleGridMapPicker(gridMapPickerActive);
			}
		}
	}
}

void PlayerInputController::SetPlayerUnitToControl(PlayerUnit* playerUnit)
{
	playerUnitToControl = playerUnit;
	GameUtils::SetActiveCamera(playerUnit->camera);
	GameUtils::SetActiveCameraTarget(playerUnit);
}
