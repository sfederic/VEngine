#include "IntuitionGainedWidget.h"
#include "Gameplay/Intuition.h"
#include "VString.h"

void IntuitionGainedWidget::Draw()
{
	Layout layout = AlignLayout(200.f, 200.f, Align::Bottom);

	FillRect(layout);
	Text(L"Intuition Gained", layout);

	layout.AddVerticalSpace(30.f);
	Text(VString::stows(intuitionToDisplay->name), layout);

	layout.AddVerticalSpace(30.f);
	Text(VString::stows(intuitionToDisplay->description), layout);
}
