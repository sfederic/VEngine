#include "vpch.h"
#include "ComboBarWidget.h"

void ComboBarWidget::Draw(float deltaTime)
{
	auto outlineLayout = PercentAlignLayout(0.5f, 0.8f, 0.9f, 0.9f);
	FillRect(outlineLayout);

	auto barLayout = PercentAlignLayout(0.5f, 0.8f, 0.6f, 0.9f);
	FillRect(barLayout, { 0.1f, 0.9f, 0.1f, 1.f });
}
