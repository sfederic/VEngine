#include "vpch.h"
#include "GearSelectionWidget.h"
#include "Gameplay/Gears/GearSystem.h"
#include "Gameplay/Gears/Gear.h"
#include "Gameplay/GameInstance.h"

void GearSelectionWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.5f, 0.5f, 0.9f, 0.9f);

	Text("Select Gears", layout);
	layout.AddVerticalSpace(30.f);

	layout = PercentAlignLayout(0.5f, 0.5f, 0.7f, 0.9f);
	auto primaryGear = GameInstance::GetGlobalProp<std::string>("PrimaryGear");
	GearSelect(layout, *primaryGear);

	layout = PercentAlignLayout(0.7f, 0.5f, 0.9f, 0.9f);
	auto secondaryGear = GameInstance::GetGlobalProp<std::string>("SecondaryGear");
	GearSelect(layout, *secondaryGear);
}

void GearSelectionWidget::GearSelect(Layout& layout, std::string& gearSelect)
{
	auto gears = GearSystem::Get().GetAllGears();
	for (auto gear : gears)
	{
		layout.PushToTop(30.f);
		const std::string gearName = gear->GetName();
		if (Button(gearName, layout))
		{
			gearSelect = gearName;
		}
		layout.AddVerticalSpace(30.f);
	}

	Rect(layout);
	Text(gearSelect, layout);
}
