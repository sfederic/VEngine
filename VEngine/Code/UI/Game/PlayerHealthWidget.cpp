#include "vpch.h"
#include "PlayerHealthWidget.h"

void PlayerHealthWidget::Draw(float deltaTime)
{
	Layout layout = AlignLayout(25.f, 25.f, Align::BottomRight);
	for (int i = 0; i < healthPoints; i++)
	{
		layout.AddHorizontalSpace(50.f);
		Image("heart_icon.png", layout);
	}
}
