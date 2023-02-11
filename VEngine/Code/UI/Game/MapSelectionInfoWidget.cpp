#include "vpch.h"
#include "MapSelectionInfoWidget.h"

void MapSelectionInfoWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.5f, 0.3f, 0.8f, 0.7f);
	FillRect(layout);
	Text(title, layout);
	layout.AddVerticalSpace(30.f);
	Text(description, layout);
}
