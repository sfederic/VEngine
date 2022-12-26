#include "vpch.h"
#include "PlayerShieldWidget.h"
#include "Actors/Game/Player.h"

void PlayerShieldWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.1f, 0.8f, 0.3f, 0.9f);
	FillRect(layout);
	auto player = Player::system.GetFirstActor();
	Text(VString::wformat(L"Shield: %.2f", player->shields), layout);
}
