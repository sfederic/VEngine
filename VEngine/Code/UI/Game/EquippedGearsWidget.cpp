#include "vpch.h"
#include "EquippedGearsWidget.h"
#include "Gameplay/Gears/GearSystem.h"
#include "Gameplay/Gears/Gear.h"

void EquippedGearsWidget::Draw(float deltaTime)
{
	auto primaryGear = GearSystem::Get().GetPrimaryGear();
	auto secondaryGear = GearSystem::Get().GetSecondaryGear();

	auto layout = PercentAlignLayout(0.1f, 0.1f, 0.2f, 0.2f);

	FillRect(layout);
	Text(primaryGear->GetName(), layout);
	layout.AddVerticalSpace(30.f);
	Text(secondaryGear->GetName(), layout);
}
