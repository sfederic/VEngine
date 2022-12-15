#include "vpch.h"
#include "ComboBarWidget.h"
#include "VString.h"

void ComboBarWidget::Draw(float deltaTime)
{
	DrawScore();
	DrawComboBar();
	
	comboBarDecreaseTimer -= deltaTime;

	if (comboBarDecreaseTimer < 0.f)
	{
		comboPoints--;
		comboBarDecreaseTimer = 1.5f;
	}
}

void ComboBarWidget::DrawScore()
{
	auto scoreLayout = PercentAlignLayout(0.5, 0.7f, 0.8f, 0.9f);
	FillRect(scoreLayout);
	Text(VString::wformat(L"Score: %d", score), scoreLayout);
}

void ComboBarWidget::DrawComboBar()
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
