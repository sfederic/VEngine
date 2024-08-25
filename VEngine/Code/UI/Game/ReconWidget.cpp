#include "vpch.h"
#include "ReconWidget.h"

void ReconWidget::Draw(float deltaTime)
{
	__super::Draw(deltaTime);

	const auto layout = PercentAlignLayout(0.45f, 0.45f, 0.55f, 0.55f);

	Image("UI/reticle.png", layout);
}
