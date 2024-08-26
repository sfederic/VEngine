#include "vpch.h"
#include "PlayerUpgradeInfoWidget.h"
#include "UI/GridLayout.h"

void PlayerUpgradeInfoWidget::Draw(float deltaTime)
{
	__super::Draw(deltaTime);

	GridLayout gridLayout;
	auto layout = PercentAlignLayout(0.3f, 0.5f, 0.5f, 0.7f);
	gridLayout.SetLayouts(1, 1, layout);

	auto indexLayout = gridLayout.GetLayoutAt(0, 0);
	FillRect(indexLayout);
	Text("Test upgrade", indexLayout);
}
