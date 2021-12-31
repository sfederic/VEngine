#include "IntuitionRecalledWidget.h"
#include "Gameplay/Intuition.h"
#include "VString.h"

void IntuitionRecalledWidget::Draw()
{
	if (recalledIntuition)
	{
		Layout layout = PercentAlignLayout(0.1f, 0.5f, 0.9f, 0.9f);

		FillRect(layout);
		Text(L"Intuition Recalled", layout);

		layout.AddVerticalSpace(30.f);
		Text(VString::stows(recalledIntuition->name), layout);

		layout.AddVerticalSpace(30.f);
		Text(VString::stows(recalledIntuition->description), layout);
	}
}
