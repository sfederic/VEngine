#include "IntuitionGainedWidget.h"
#include "Gameplay/Intuition.h"
#include "VString.h"

void IntuitionGainedWidget::Draw(float deltaTime)
{
	Layout layout = PercentAlignLayout(0.1f, 0.35f, 0.9f, 0.5f);

	FillRect(layout);
	Text(L"Intuition Gained", layout);

	layout.AddVerticalSpace(30.f);
	Text(VString::stows(intuitionToDisplay->name), layout);
}
