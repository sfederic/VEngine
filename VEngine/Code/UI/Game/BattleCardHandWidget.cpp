#include "vpch.h"
#include "BattleCardHandWidget.h"
#include "Actors/Game/Player.h"
#include "Gameplay/BattleCards/BattleCard.h"

void BattleCardHandWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.1f, 0.8f, 0.15f, 0.9f);

	for (auto card : Player::system.GetFirstActor()->battleCardsInHand)
	{
		if (ImageButton(card->imageFilename, layout))
		{
			card->Activate();
		}

		layout.AddHorizontalSpace(75.f);
	}
}
