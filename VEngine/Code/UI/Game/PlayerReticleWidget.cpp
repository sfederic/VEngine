#include "vpch.h"
#include "PlayerReticleWidget.h"

void PlayerReticleWidget::Draw(float deltaTime)
{
	auto layout = CenterLayoutOnScreenSpaceCoords(50, 50);
	Image("UI/reticle.png", layout);
}
