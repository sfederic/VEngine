#include "PlayerActionBarWidget.h"

void PlayerActionBarWidget::Tick(float deltaTime)
{
	D2D1_RECT_F rect = AlignLayout(150.f, 15.f, Align::Bottom);
	D2D1_RECT_F iconRect = rect;
	iconRect.right = rect.left + 20.f;

	for (int i = 0; i < actionPoints; i++)
	{
		iconRect.left += 20.f;
 		iconRect.right += 20.f;
		FillRect(iconRect, {0.f, 0.8f, 0.1f, 1.f}, 0.5f);

		//Padding
		iconRect.left += 5.f;
		iconRect.right += 5.f;
	}
}
