#include "vpch.h"
#include "PlayerUpgradeInfoWidget.h"
#include "UI/GridLayout.h"
#include "Gameplay/GameInstance.h"

void PlayerUpgradeInfoWidget::Draw(float deltaTime)
{
	__super::Draw(deltaTime);

	GridLayout gridLayout;
	auto layout = PercentAlignLayout(0.3f, 0.5f, 0.5f, 0.7f);
	gridLayout.SetLayouts(1, 1, layout);

	auto upgradeNames = GameInstance::GetAllPlayerUpgradeNames();
	for (auto& upgradeName : upgradeNames)
	{
		//Todo: this won't work with multiple rows as is, but just for testing.
		auto indexLayout = gridLayout.GetLayoutAt(0, 0);
		FillRect(indexLayout);
		Text(upgradeName, indexLayout);
	}
}
