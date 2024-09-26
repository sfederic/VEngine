#pragma once

#include "../Widget.h"

class ScanningEnemyAlertWidget : public Widget
{
public:
	ScanningEnemyAlertWidget() : Widget("ScanningEnemyAlertWidget") {}
	void Draw(float deltaTime) override;
};
