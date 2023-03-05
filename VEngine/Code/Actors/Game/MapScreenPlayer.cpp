#include "vpch.h"
#include "MapScreenPlayer.h"

void MapScreenPlayer::Start()
{
	SetCameraAsActive();
}

void MapScreenPlayer::Tick(float deltaTime)
{
	MovementInput(deltaTime);
}
