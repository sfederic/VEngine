#include "IntuitionRecalledWidget.h"
#include "Gameplay/Intuition.h"
#include "VString.h"

void IntuitionRecalledWidget::Draw()
{
	if (recalledIntuition)
	{
		Layout layout = PercentAlignLayout(0.2f, 0.1f, 0.8f, 0.4f);

		Text(L"Intuition Recalled", layout);

		layout.AddVerticalSpace(30.f);
		Text(VString::stows(recalledIntuition->name), layout);

		layout.AddVerticalSpace(30.f);
		Text(VString::stows(recalledIntuition->description), layout);
	}
}
