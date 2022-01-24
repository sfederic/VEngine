#include "BulletWidget.h"

void BulletWidget::Draw(float deltaTime)
{
	Layout layout = AlignLayout(25.f, 25.f, Align::BottomLeft);
	for (int i = 0; i < 2; i++)
	{
		Image("bullet.png", layout);
		layout.AddHorizontalSpace(50.f);
	}
}
