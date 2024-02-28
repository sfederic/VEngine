#include "vpch.h"
#include "QuickThoughtWidget.h"
#include "Actors/Game/Player.h"

void QuickThoughtWidget::Draw(float deltaTime)
{
	__super::Draw(deltaTime);
}

void QuickThoughtWidget::OnRemoveFromViewport()
{
	Player::system.GetOnlyActor()->SetQuickThoughtOff();
}
