#include "vpch.h"
#include "PlayerReticleWidget.h"

void PlayerReticleWidget::Draw(float deltaTime)
{
	auto layout = CenterLayoutOnScreenSpaceCoords(200, 200);
	Image("UI/reticle.png", layout);
}
