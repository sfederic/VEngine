#include "vpch.h"
#include "ScanWidget.h"

void ScanWidget::Draw(float deltaTime)
{
	Layout layout = PercentAlignLayout(0.3f, 0.65f, 0.7f, 0.95f);

	FillRect(layout);
	Text(scanInfoText, layout);
}

void ScanWidget::ResetValues()
{
	scanInfoText.clear();
}
