#include "vpch.h"
#include "PlayerReticleWidget.h"

void PlayerReticleWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.45f, 0.4f, 0.55f, 0.6f);
	Image("UI/reticle.png", layout);
}
