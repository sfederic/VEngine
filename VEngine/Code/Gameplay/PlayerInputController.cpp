#include "vpch.h"
#include "PlayerInputController.h"
#include "Input.h"
#include "Actors/Game/PlayerUnit.h"

PlayerInputController playerInputController;

void PlayerInputController::Tick(float deltaTime)
{
	if (playerUnitToControl)
	{
		playerUnitToControl->ControllerInput(deltaTime);
	}
}
