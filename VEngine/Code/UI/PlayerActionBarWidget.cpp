#include "vpch.h"
#include "PlayerActionBarWidget.h"

void PlayerActionBarWidget::Draw(float deltaTime)
{
	Layout layout = AlignLayout(150.f, 15.f, Align::Bottom);
	layout.PushToLeft();
	layout.rect.right += 20.f;

	for (int i = 0; i < actionPoints; i++)
	{
		layout.AddHorizontalSpace(20.f);
		FillRect(layout, {0.f, 0.8f, 0.1f, 1.f}, 0.5f);

		//Padding
		layout.AddHorizontalSpace(5.f);
	}
}
