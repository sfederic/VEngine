#include "vpch.h"
#include "BattleCardHandWidget.h"
#include "Actors/Game/Player.h"
#include "Gameplay/BattleCard.h"

void BattleCardHandWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.1f, 0.8f, 0.15f, 0.9f);

	for (auto& battleCard : Player::system.GetFirstActor()->battleCardsInHand)
	{
		FillRect(layout);

		layout.AddHorizontalSpace(75.f);
	}
}
