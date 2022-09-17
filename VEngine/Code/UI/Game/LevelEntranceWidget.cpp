#include "vpch.h"
#include "LevelEntranceWidget.h"

void LevelEntranceWidget::Draw(float deltaTime)
{
	Layout layout = PercentAlignLayout(0.3f, 0.6f, 0.7f, 0.9f);
	FillRect(layout);
	Text(levelName, layout);

	layout.AddVerticalSpace(50.f);
	Text(L"Press 'Enter' to enter level.", layout);
}
