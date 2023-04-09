#include "vpch.h"
#include "PlayerStatusWidget.h"

void PlayerStatusWidget::Draw(float deltaTime)
{
	int sx, sy;
	GetScreenSpaceCoords(sx, sy);
	auto layout = CenterLayoutOnScreenSpaceCoords(75, 75, sx, sy - 150);
	Image("UI/fatigue.png", layout);
}
