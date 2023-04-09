#include "vpch.h"
#include "PlayerStatusWidget.h"

void PlayerStatusWidget::Draw(float deltaTime)
{
	auto layout = CenterLayoutOnScreenSpaceCoords(100, 100);
	Image("UI/fatigue.png", layout);
}
