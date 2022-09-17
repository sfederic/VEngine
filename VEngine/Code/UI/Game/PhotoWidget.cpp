#include "vpch.h"
#include "PhotoWidget.h"

void PhotoWidget::Draw(float deltaTime)
{
	Layout layout = PercentAlignLayout(0.1f, 0.5f, 0.3f, 0.7f);
	Image(photoFilename, layout);
}
