#include "IntuitionGainedWidget.h"
#include "Gameplay/Intuition.h"
#include "VString.h"

void IntuitionGainedWidget::Draw(float deltaTime)
{
	Layout layout = PercentAlignLayout(0.f, 0.f, 1.f, 0.1f);

	FillRect(layout);
	Text(L"Intuition Gained", layout);

	layout.AddVerticalSpace(30.f);
	Text(VString::stows(intuitionToDisplay->name), layout);
}
