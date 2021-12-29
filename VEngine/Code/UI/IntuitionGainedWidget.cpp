#include "IntuitionGainedWidget.h"
#include "Gameplay/Intuition.h"
#include "VString.h"

void IntuitionGainedWidget::Draw()
{
	D2D1_RECT_F layout = AlignLayout(200.f, 200.f, Align::Bottom);

	FillRect(layout);
	Text(L"Intuition Gained", layout);

	layout.top += 30.f;
	layout.bottom += 30.f;

	Text(VString::stows(intuitionToDisplay->name), layout);

	layout.top += 30.f;
	layout.bottom += 30.f;

	Text(VString::stows(intuitionToDisplay->description), layout);
}
