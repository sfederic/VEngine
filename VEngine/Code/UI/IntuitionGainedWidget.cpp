#include "IntuitionGainedWidget.h"
#include "Gameplay/Intuition.h"
#include "VString.h"

void IntuitionGainedWidget::Draw()
{
	Layout layout = AlignLayout(200.f, 50.f, Align::Top);

	FillRect(layout);
	Text(L"Intuition Gained", layout);

	layout.AddVerticalSpace(30.f);
	Text(VString::stows(intuitionToDisplay->name), layout);
}
