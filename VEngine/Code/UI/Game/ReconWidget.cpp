#include "vpch.h"
#include "ReconWidget.h"

void ReconWidget::Draw(float deltaTime)
{
	__super::Draw(deltaTime);

	Image("UI/reticle.png", PercentAlignLayout(0.45f, 0.45f, 0.55f, 0.55f));

	auto textLayout = PercentAlignLayout(0.35f, 0.6f, 0.65f, 0.9f);
	FillRect(textLayout);
	Text(reconDisplayText, textLayout);
}
