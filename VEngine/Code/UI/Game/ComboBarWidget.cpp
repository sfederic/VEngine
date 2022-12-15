#include "vpch.h"
#include "ComboBarWidget.h"

void ComboBarWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.5f, 0.8f, 0.9f, 0.9f);
	layout.PushToLeft();
	layout.rect.right += 30.f;

	for (int i = 0; i < comboPoints; i++)
	{
		layout.AddHorizontalSpace(30.f);
		FillRect(layout, { 0.1f, 0.9f, 0.3f, 1.f });

		//Padding
		layout.AddHorizontalSpace(5.f);
	}
}
