#include "PlayerActionBarWidget.h"

void PlayerActionBarWidget::Tick(float deltaTime)
{
	D2D1_RECT_F rect = AlignLayout(150.f, 25.f, Align::Bottom);
	D2D1_RECT_F iconRect = rect;
	iconRect.right = rect.left;

	for (int i = 0; i < *actionPoints; i++)
	{
		iconRect.right += 20.f;
		Rect(iconRect);
	}
}
