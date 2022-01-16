#include "WeaponDisplayWidget.h"

void WeaponDisplayWidget::Draw(float deltaTime)
{
	FillRect(PercentAlignLayout(0.1f, 0.8f, 0.5f, 0.9f));

	auto layout = PercentAlignLayout(0.1f, 0.8f, 0.2f, 0.9f);

	Button(L"Spear", layout);
	layout.AddHorizontalSpace(120.f);

	Button(L"Sword", layout);
	layout.AddHorizontalSpace(120.f);

	Button(L"Shield", layout);
	layout.AddHorizontalSpace(120.f);
}
