#include "vpch.h"
#include "PlayerHealthWidget.h"
#include "Actors/Game/Player.h"

void PlayerHealthWidget::Draw(float deltaTime)
{
	const int playerHealthPoints = Player::system.GetFirstActor()->healthPoints;

	Layout layout = AlignLayout(25.f, 25.f, Align::BottomRight);
	for (int i = 0; i < playerHealthPoints; i++)
	{
		layout.AddHorizontalSpace(50.f);
		Image("UI/heart_icon.png", layout);
	}
}
