#include "vpch.h"
#include "ComboBarWidget.h"

void ComboBarWidget::Draw(float deltaTime)
{
	auto outlineLayout = PercentAlignLayout(0.5f, 0.8f, 0.9f, 0.9f);
	Rect(outlineLayout);

	auto barLayout = PercentAlignLayout(0.5f, 0.8f, 0.7f, 0.9f);
	FillRect(barLayout, { 0.8f, 0.9f, 0.1f, 1.f });
}
