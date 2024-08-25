#include "vpch.h"
#include "ReconWidget.h"

void ReconWidget::Draw(float deltaTime)
{
	__super::Draw(deltaTime);

	const auto layout = PercentAlignLayout(0.4f, 0.4f, 0.6f, 0.6f);

	Image("UI/reticle.png", layout);
}
